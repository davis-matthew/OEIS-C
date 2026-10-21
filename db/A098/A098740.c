/*
 * A098740  Start with the sequence of natural numbers S(0)={1,2,3,...} and
 *          define, for i>0, S(i)=D(i)S(i-1), where D(i)A denotes the
 *          operation of deleting the a(1+[i/2])th term of
 *          A={a(1),a(2),a(3),...}. E.g. D(3){1,2,4,6,9,10,...} means to
 *          delete the a(1+[3/2])th = 2nd term of {1,2,4,9,10,...}, giving
 *          {1,4...
 * https://oeis.org/A098740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   fenwick_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098740:  a(n) is the n‑th integer that survives the deletion process
    defined by S(i) = D(i)S(i-1), where D(i) deletes the
    a(1+⌊i/2⌋)-th element of the current list A.  The function below
    simulates the process up to a sufficient bound, caches the first
    20 000 terms, and returns the requested one. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fenwick_add.h"

static int fenwick_find_kth(int *bit, int n, int k)
{
    int idx = 0;
    int bitMask = 1;
    while (bitMask <= n) bitMask <<= 1;
    bitMask >>= 1;
    for (; bitMask; bitMask >>= 1) {
        int next = idx + bitMask;
        if (next <= n && bit[next] < k) {
            idx  = next;
            k   -= bit[next];
        }
    }
    return idx + 1;
}

/* parameters for pre‑computation --------------------------------------------- */
#define MAX_VAL      400000   /* maximum number considered in the simulation  */
#define CACHE_SIZE   20000    /* how many terms to store for answering queries */

/* static cache --------------------------------------------------------------- */
static seq_t seq_cache[CACHE_SIZE];
static idx_t seq_cache_len = 0;

/* one‑time precomputation ---------------------------------------------------- */
static void seq_precompute(void)
{
    if (seq_cache_len > 0) return;      /* already done */

    const int N = MAX_VAL;
    int *bit = (int *)calloc((size_t)N + 1, sizeof(int));
    char *alive = (char *)calloc((size_t)N + 1, sizeof(char));
    if (!bit || !alive) return;         /* allocation failure – leave cache empty */

    /* initialise Fenwick tree and alive array */
    for (int i = 1; i <= N; ++i) {
        alive[i] = 1;
        fenwick_add(bit, N, i, 1);
    }

    int size = N;            /* number of alive numbers ≤ N */
    for (idx_t step = 1; ; ++step) {
        idx_t k = 1 + step / 2;   /* position to look at */
        if (k > (idx_t)size) break;   /* subsequent deletions would be beyond our range */

        int val_k = fenwick_find_kth(bit, N, (int)k);   /* a(k) */

        if (val_k <= size) {             /* deletion also inside our range */
            int delIdx = fenwick_find_kth(bit, N, val_k);
            fenwick_add(bit, N, delIdx, -1);
            alive[delIdx] = 0;
            --size;
        }
        /* else: deletion target is >N, no effect on our stored range */
    }

    /* collect survivors into cache */
    for (int i = 1; i <= N && seq_cache_len < CACHE_SIZE; ++i)
        if (alive[i])
            seq_cache[seq_cache_len++] = (seq_t)i;

    free(bit);
    free(alive);
}

/*  public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n = 0, safe default */
    seq_precompute();                  /* ensure cache is ready */
    if (n > seq_cache_len) return 0;   /* out of precomputed range */
    return seq_cache[n - 1];           /* 1‑based indexing */
}
