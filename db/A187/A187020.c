/*
 * A187020  McKay-Thompson series of class 36D for the Monster group with a(0)
 *          = 1.
 * https://oeis.org/A187020
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 169, u64 n <= 578,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187020
 *  McKay‑Thompson series of class 36D for the Monster group.
 *  The sequence is the Euler transform of the periodic 36‑term
 *  indicator array (1,1,1,0,1,1,1,0,0,1,1,0,1,1,1,0,
 *  1,0,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,1,1,0,…) –
 *  i.e. a(n) = number of partitions of n+1 into parts k for which
 *  k ≡ {1,2,3,5,6,7,10,11,13,14,15,17,19,21,22,23,25,26,28,30,31,32,33,35} (mod 36).
 *  The function below computes a(n) for all n ≥ –1 (a(–1)=1) by
 *  dynamic programming; it requires only additions on type seq_t.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache of partition counts dp[i] = s(i) for i = 0…current_size-1  */
static seq_t *dp = NULL;
static size_t dp_size = 0;

/* period‑36 indicator array (1 for allowed part sizes, 0 otherwise) */
static const unsigned char period[36] = {
    1,1,1,0,1,1,1,0,0,
    1,1,0,1,1,1,0,1,0,
    1,0,1,1,1,0,1,1,0,
    0,1,1,1,0,1,1,1,0
};

/*   ensure that dp[0 … n+1] is defined   */
static void ensure_dp(idx_t n)
{
    if (n < -1) return;                     /* nothing needed for negative indices */

    size_t need = (size_t)(n + 2);           /* highest index needed is n+1   */
    if (need == 0) need = 1;                /* shooting guard for -1          */

    if (dp == NULL || need > dp_size) {
        /* Allocate or reallocate cache */
        seq_t *newdp = (seq_t*)malloc(need * sizeof(seq_t));
        if (!newdp) return;                /* allocation failed – leave unchanged  */

        /* initialise to zero and set dp[0]=1 (s(0)=1) */
        for (size_t i = 0; i < need; ++i) newdp[i] = 0;
        newdp[0] = (seq_t)1;

        /* dynamic programming for partitions with allowed part sizes */
        for (size_t k = 1; k < need; ++k) {
            if (period[(k-1) % 36]) {       /* k allowed?   */
                for (size_t i = k; i < need; ++i) {
                    newdp[i] += newdp[i - k];
                }
            }
        }

        /* replace old cache if any */
        free(dp);
        dp = newdp;
        dp_size = need;
    }
}

seq_t a(idx_t n)
{
    if (n < -1) return 0;                     /* indices below the offset are undefined */
    ensure_dp(n);
    return dp[(size_t)(n + 1)];               /* a(n) = s(n+1) */
}
