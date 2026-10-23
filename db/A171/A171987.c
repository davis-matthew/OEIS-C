/*
 * A171987  Best explained by example: in the binary representation, start
 *          with 10000, then add 1 and push the 1 to the left: 10001, 10010,
 *          10100, 11000, then add another one, 11001, 11010, 11100, etc:
 *          11101, 11110, 11111. Then proceed with the next length of numbers:
 *          100000, etc.
 * https://oeis.org/A171987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2^k-1)*2^l + 2^m, k≥0, l>m≥0, sorted increasingly
 * Range:     u8 n <= 64, u16 n <= 695, u32 n <= 5063, u64 n <= 5489,
 *            u128 n <= 5489, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

/* State for each possible leading block length k  (k=0..KMAX) */
typedef struct {
    seq_t value;   /* current value for this k */
    int k;         /* leading ones count (k==0 is special) */
    int l;         /* current l (unused for k==0) */
    int m;         /* current m (unused for k==0) */
    int e;         /* current exponent for k==0 (powers of two) */
} state_t;

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n>=1 in normal use */

    /* Upper bound for k that can contribute numbers fitting into seq_t */
    const int KMAX = 60;             /* 2^(KMAX+1)-1 < UINT64_MAX for seq_t 64‑bit */

    state_t states[KMAX + 1];
    /* Initialise states for all k */
    for (int k = 0; k <= KMAX; ++k) {
        if (k == 0) {                      /* powers of 2 */
            states[k].k   = 0;
            states[k].e   = 0;
            states[k].value = pow2(0);      /* 1 */
        } else {                            /* k >= 1 */
            states[k].k   = k;
            states[k].l   = 1;
            states[k].m   = 0;
            seq_t t = pow2(k);              /* 2^k */
            t -= 1;                         /* 2^k - 1 */
            states[k].value = (t << 1) + pow2(0);  /* (2^k-1)<<1 + 1 */
        }
    }

    seq_t result = 0;
    for (idx_t count = 1; count <= n; ++count) {
        /* Find the state with the minimal current value */
        int minIdx = 0;
        seq_t minVal = states[0].value;
        for (int k = 1; k <= KMAX; ++k) {
            if (states[k].value < minVal) {
                minVal = states[k].value;
                minIdx = k;
            }
        }

        result = minVal;
        state_t *s = &states[minIdx];

        /* Advance this state to the next number of the same k */
        if (s->k == 0) {               /* powers of 2 */
            s->e++;
            s->value = pow2(s->e);
        } else {                       /* k >= 1 */
            if (s->m + 1 < s->l) {
                s->m++;
            } else {
                s->l++;
                s->m = 0;
            }
            seq_t t = pow2(s->k);
            t -= 1;                      /* 2^k - 1 */
            s->value = (t << s->l) + pow2(s->m);
        }
    }

    return result;
}
