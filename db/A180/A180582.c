/*
 * A180582  Number of Hamiltonian cycles in C_6 X P_n.
 * https://oeis.org/A180582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -12*a(n-7) - 32*a(n-6) - 36*a(n-5) - 28*a(n-4) +
 *            10*a(n-3) + 9*a(n-1) for n > 8 Initial values (offset 1):
 *            a(1)=1, a(2)=8, a(3)=86, a(4)=776, a(5)=7010, a(6)=63674,
 *            a(7)=578090, a(8)=5247824
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define MAX_CACHE_N 20000          /* enough for all tests (≈10 000) */

static seq_t cache[MAX_CACHE_N + 1];
static idx_t computed_up_to = 0;   /* last index that has a defined value */

/* initialise the first eight values */
static void init_base(void)
{
    static const seq_t init[9] = {
        0,                /* unused 0‑th slot, keeps cache[0] = 0 */
        (seq_t)1,         /* a(1) */
        (seq_t)8,         /* a(2) */
        (seq_t)86,        /* a(3) */
        (seq_t)776,       /* a(4) */
        (seq_t)7010,      /* a(5) */
        (seq_t)63674,     /* a(6) */
        (seq_t)578090,    /* a(7) */
        (seq_t)5247824    /* a(8) */
    };
    for (size_t i = 1; i <= 8; ++i)
        cache[i] = init[i];
    cache[0] = 0;
    computed_up_to = 8;
}

/* Compute a(n) for n > MAX_CACHE_N by rolling recurrence */
static seq_t compute_large(idx_t n)
{
    /* buffer holds a_{k-7} … a_{k-1} for the current k */
    seq_t buf[7];
    /* initialise buffer for k = 9: a2 … a8 */
    buf[0] = (seq_t)8;          /* a2 */
    buf[1] = (seq_t)86;         /* a3 */
    buf[2] = (seq_t)776;        /* a4 */
    buf[3] = (seq_t)7010;       /* a5 */
    buf[4] = (seq_t)63674;      /* a6 */
    buf[5] = (seq_t)578090;     /* a7 */
    buf[6] = (seq_t)5247824;    /* a8 */

    seq_t val = 0;
    for (idx_t k = 9; k <= n; ++k) {
        seq_t sumPos = (seq_t)9 * buf[6] + (seq_t)10 * buf[4];
        seq_t sumNeg = (seq_t)12 * buf[0] +
                       (seq_t)28 * buf[3] +
                       (seq_t)36 * buf[2] +
                       (seq_t)32 * buf[1];
        val = sumPos - sumNeg;

        /* shift buffer left by one and store new value at end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = val;
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (n <= MAX_CACHE_N) {
        if (computed_up_to == 0)
            init_base();

        if (n > computed_up_to) {
            for (idx_t i = computed_up_to + 1; i <= n; ++i) {
                size_t idx = (size_t)i;
                seq_t sumPos = (seq_t)9 * cache[idx-1] + (seq_t)10 * cache[idx-3];
                seq_t sumNeg = (seq_t)12 * cache[idx-7] +
                               (seq_t)28 * cache[idx-4] +
                               (seq_t)36 * cache[idx-5] +
                               (seq_t)32 * cache[idx-6];
                cache[idx] = sumPos - sumNeg;
            }
            computed_up_to = n;
        }
        return cache[(size_t)n];
    }

    /* For larger n, compute on the fly (no caching) */
    return compute_large(n);
}
