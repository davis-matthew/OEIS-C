/*
 * A015084  Carlitz-Riordan q-Catalan numbers for q=3.
 * https://oeis.org/A015084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{n-1} 3^i * a(i) * a(n-1-i); a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation memoises the values of a(n) and of 3^k.
    It works for any seq_t/idx_t definitions provided by the build
    environment and stays well below the 2‑second per call limit for
    n up to at least 10 000. */

#include <stdlib.h>

/* Helper: grow an array of seq_t elements to at least 'needed' capacity.  */
static void grow_seq_t_array(seq_t **arr, idx_t *cap, idx_t needed)
{
    if (needed <= *cap) return;
    idx_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* keep behaviour simple */
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *memo   = NULL;   /* memo[i] = a(i)      */
    static idx_t memo_cap = 0;     /* current allocated size */
    static idx_t memo_sz  = 0;     /* number of elements computed */

    static seq_t *pow3   = NULL;   /* pow3[i] = 3^i       */
    static idx_t pow3_cap = 0;     /* capacity of pow3    */
    static idx_t pow3_sz  = 0;     /* number of powers computed */

    /* initialise memo and pow3 on first call */
    if (memo_sz == 0) {
        /* Allocate room for the first element (a(0)=1) */
        grow_seq_t_array(&memo, &memo_cap, 1);
        memo[0] = (seq_t)1;
        memo_sz = 1;                /* next index to compute is 1 */
    }
    if (pow3_sz == 0) {
        grow_seq_t_array(&pow3, &pow3_cap, 1);
        pow3[0] = (seq_t)1;        /* 3^0 = 1 */
        pow3_sz = 1;               /* next exponent to compute is 1 */
    }

    /* compute all missing terms up to n */
    for (idx_t cur = memo_sz; cur <= n; ++cur) {
        /* ---------  make sure powers of 3 up to cur exist --------- */
        while (pow3_sz <= cur) {                /* need at least 3^(cur) */
            grow_seq_t_array(&pow3, &pow3_cap, pow3_sz + 1);
            pow3[pow3_sz] = pow3[pow3_sz - 1] * (seq_t)3;
            ++pow3_sz;
        }

        /* ---------  recurrence a(cur) = Σ_{i=0}^{cur-1} 3^i a(i) a(cur-1-i)  --------- */
        seq_t sum = 0;
        for (idx_t i = 0; i < cur; ++i) {
            sum += pow3[i] * memo[i] * memo[cur - 1U - i];
        }

        /* make sure memo has space for a(cur) */
        grow_seq_t_array(&memo, &memo_cap, cur + 1);
        memo[cur] = sum;
        if (cur + 1 > memo_sz) memo_sz = cur + 1;
    }

    return memo[n];
}
