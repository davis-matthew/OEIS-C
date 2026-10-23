/*
 * A158524  Choulet-Curtz triangle with T(0,0)=1, T(n,n)=T(n,0).
 * https://oeis.org/A158524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (r,c)-entry of the Choulet‑Curtz triangle defined by
 *            T(0,0)=1, T(n,k)=T(n-1,k)+T(k-1,k-1) (k≥1, n>k),
 *            T(n,n)=T(n,0)=Σ_{i=0}^{n-1}T(n-1,i). The sequence a(n) is the
 *            triangle read row‑major with offset 0
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* a(0) … a(n) stored sequentially          */
static size_t cache_cap  = 0;
static size_t cache_len  = 0;      /* number of a-values stored                */

static seq_t *prevRow    = NULL;   /* previous triangle row (size = currentRow+1) */
static size_t prev_cap   = 0;      /* capacity of prevRow                        */
static seq_t *rowSums    = NULL;   /* sums of each row (rowSums[i]=ΣT(i,⋅))       */
static size_t sums_cap   = 0;      /* capacity of rowSums                        */

static idx_t  currentRow = 0;      /* highest row fully computed                 */
static idx_t  maxIndex   = 0;      /* maximum index currently cached             */

/* Ensure static structures are initialised.  */
static void init(void)
{
    if (cache != NULL)
        return;

    /* enough capacity for first few elements */
    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);

    /* row 0 */
    prev_cap = 1;
    prevRow = (seq_t *)malloc(prev_cap * sizeof(seq_t));
    if (!prevRow) exit(1);
    prevRow[0] = (seq_t)1;

    rowSums = (seq_t *)malloc(sizeof(seq_t));
    if (!rowSums) exit(1);
    rowSums[0] = (seq_t)1;

    /* cache[0] = T(0,0) */
    cache[0] = (seq_t)1;
    cache_len = 1;
    currentRow = 0;
    maxIndex   = 0;

    sums_cap = 1;
}

/* Grow cache to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *newbuf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newbuf) exit(1);
    cache = newbuf;
    cache_cap = new_cap;
}

/* Grow rowSums to hold at least `needed` rows. */
static void grow_rowSums(size_t needed)
{
    if (needed <= sums_cap)
        return;
    size_t new_cap = sums_cap ? sums_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *newbuf = (seq_t *)realloc(rowSums, new_cap * sizeof(seq_t));
    if (!newbuf) exit(1);
    rowSums = newbuf;
    sums_cap = new_cap;
}

/* Compute the next triangle row (currentRow+1) and extend the cache. */
static void compute_next_row(void)
{
    idx_t next = currentRow + 1;                    /* new row index */
    idx_t len  = (idx_t)(next + 1);                 /* elements in new row */

    /* allocate new row buffer */
    seq_t *newRow = (seq_t *)malloc(len * sizeof(seq_t));
    if (!newRow) exit(1);

    /* sum of previous row */
    seq_t sumPrev = rowSums[currentRow];
    newRow[0] = sumPrev;                            /* T(next,0) */

    /* compute middle entries */
    for (idx_t c = 1; c < next; ++c) {
        seq_t prevVal = (c < prev_cap) ? prevRow[c] : 0;  /* safe access */
        seq_t diag    = (c == 1) ? (seq_t)1 : rowSums[c-2]; /* T(c-1,c-1) */
        newRow[c] = prevVal + diag;
    }

    /* last element */
    newRow[next] = sumPrev;                         /* T(next,next) */

    /* compute sum of new row */
    seq_t sumCur = 0;
    for (idx_t j = 0; j < len; ++j)
        sumCur += newRow[j];

    /* extend cache with new row's values */
    idx_t offset = (idx_t)(next * (next + 1) / 2);
    grow_cache(offset + len);
    for (idx_t j = 0; j < len; ++j)
        cache[offset + j] = newRow[j];
    cache_len = offset + len;

    /* update running structures */
    free(prevRow);
    prevRow = newRow;
    prev_cap = len;

    grow_rowSums(next + 1);
    rowSums[next] = sumCur;

    currentRow = next;
    maxIndex   = cache_len - 1;
}

/* Find the largest row r such that r*(r+1)/2 <= n.  */
static idx_t row_for_index(idx_t n)
{
    idx_t r = 0;
    while ( (__int128)(r + 1) * (r + 2) / 2 <= n )
        ++r;
    return r;
}

/* Public sequence routine.  */
seq_t a(idx_t n)
{
    init();

    if (n < cache_len)            /* already computed */
        return cache[n];

    /* compute all missing rows up to the one containing n */
    idx_t targetRow = row_for_index(n);
    while (currentRow < targetRow)
        compute_next_row();

    /* at this point cache contains up to targetRow */
    return cache[n];
}
