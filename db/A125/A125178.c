/*
 * A125178  Triangle read by rows: T(n,0)=B(n) (the Bell numbers, A000110(n)),
 *          T(n,k)=0 for k < 0 or k > n, T(n,k) = T(n-1,k) + T(n-1,k-1) for n
 *          >= 1, 0 <= k <= n.
 * https://oeis.org/A125178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A125178: a(n) is the nth element of the triangle read by rows.
   Triangle T(n,k) is defined by
     T(0,0)=1,
     T(n,0)=Bell(n) for n>=1,
     T(n,k)=T(n-1,k)+T(n-1,k-1) for 1<=k<=n.
   Sequence a(n) is obtained by linear indexing of this triangle. */

#include <stddef.h>
#include <stdlib.h>

/* multiplicative binomial coefficient, result in seq_t */
static inline seq_t binom_val(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* static storage for Bell numbers and the triangle rows */
static seq_t *bell = NULL;          /* Bell[0..] */
static size_t bell_cnt = 0;
static seq_t **rows = NULL;         /* rows[0..] each pointing to array of size r+1 */
static size_t rows_cnt = 0;

/* initialize row 0 */
static void init(void)
{
    /* Bell(0) = 1 */
    bell = (seq_t *)malloc(sizeof(seq_t));
    bell[0] = (seq_t)1;
    bell_cnt = 1;

    /* row 0 : [1] */
    rows = (seq_t **)malloc(sizeof(seq_t *));
    rows[0] = (seq_t *)malloc(sizeof(seq_t));
    rows[0][0] = (seq_t)1;
    rows_cnt = 1;
}

/* compute Bell numbers up to index n (inclusive) */
static void compute_bell_to(idx_t n)
{
    while (bell_cnt <= (size_t)n)
    {
        idx_t idx = bell_cnt;              /* compute Bell(idx) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < idx; ++k)
        {
            seq_t b = bell[k];
            seq_t c = binom_val(idx - 1, k);
            sum = sum + c * b;
        }

        bell = (seq_t *)realloc(bell, (bell_cnt + 1) * sizeof(seq_t));
        bell[bell_cnt] = sum;
        ++bell_cnt;
    }
}

/* compute rows up to row r (inclusive) */
static void compute_rows_to(idx_t r)
{
    while (rows_cnt <= (size_t)r)
    {
        idx_t cur = rows_cnt;          /* compute row cur */
        /* ensure Bell(cur) is available */
        compute_bell_to(cur);

        seq_t *row = (seq_t *)malloc((cur + 1) * sizeof(seq_t));

        /* first element */
        row[0] = bell[cur];

        /* remaining elements */
        for (idx_t k = 1; k <= cur; ++k)
        {
            seq_t left = (k <= cur - 1) ? rows[cur - 1][k] : (seq_t)0;
            seq_t right = rows[cur - 1][k - 1];
            row[k] = left + right;
        }

        rows = (seq_t **)realloc(rows, (rows_cnt + 1) * sizeof(seq_t *));
        rows[rows_cnt] = row;
        ++rows_cnt;
    }
}

/* public function required by build */
seq_t a(idx_t n)
{
    /* lazily init */
    if (!bell)
        init();

    /* find row R such that offset(R) <= n < offset(R)+R+1 */
    idx_t R = 0;
    while ((R * (R + 1)) / 2 <= n)
        ++R;
    --R;                                 /* now R satisfies offset <= n */

    compute_rows_to(R);

    seq_t offset = (seq_t)R * ((seq_t)R + 1) / 2;
    idx_t k = (idx_t)(n - offset);
    return rows[R][k];
}
