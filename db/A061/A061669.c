/*
 * A061669  a(n) = n*(mu(n) + 1), where mu(n) is the Moebius function A008683.
 * https://oeis.org/A061669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n * (mu(n) + 1) where mu is the Möbius function
 * Range:     u8 n <= 128, u16 n <= 9300, u32 n <= 9300, u64 n <= 9300,
 *            u128 n <= 9300, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t _pr[64];
static uint64_t _ex[64];
static size_t   _cnt;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset starts at 1; 0 is unused */

    int mu;
    if (n == 1) {
        mu = 1;                           /* mu(1) = 1 */
    } else {
        _cnt = 0;
        factor_u64((uint64_t)n, _pr, _ex, &_cnt);

        int squarefree = 1;
        for (size_t i = 0; i < _cnt; ++i) {
            if (_ex[i] > 1) {             /* repeated prime factor */
                squarefree = 0;
                break;
            }
        }

        if (!squarefree) {
            mu = 0;                       /* not square‑free */
        } else {
            mu = (_cnt % 2 == 0) ? 1 : -1;/* (-1)^k with k = number of factors */
        }
    }

    seq_t rn = (seq_t)n;
    if (mu == 0)      return rn;          /* a(n) = n when mu(n)=0 */
    if (mu == 1)      return rn * 2;      /* a(n) = 2n when mu(n)=1 */
    return 0;                            /* a(n) = 0 when mu(n)=-1 */
}
