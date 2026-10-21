/*
 * A082691  a(1)=1, a(2)=2, then if the first 3*2^k-1 terms are a(1), a(2),
 *          ..., a(3*2^k - 1), the first 3*2^(k+1)-1 terms are a(1), a(2),
 *          ..., a(3*2^k - 1), a(1), a(2), ..., a(3*2^k - 1), a(3*2^k-1) + 1.
 * https://oeis.org/A082691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   defined by recursive concatenation. For k ≥ 0 let L_k = 3·2^k –
 *            1. For n with L_{k‑1} < n ≤ L_k, - if n == L_k → a(n) =
 *            a(L_{k‑1}) + 1 - if L_{k‑1} < n < L_k → a(n) = a(n – L_{k‑1})
 *            (second copy) - if n ≤ L_{k‑1} → a(n) = a(n) (first copy)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t base_vals[2] = { (seq_t)1, (seq_t)2 };

seq_t a(idx_t n)
{
    if (n == 1) return base_vals[0];
    if (n == 2) return base_vals[1];

    seq_t inc = 0;
    idx_t cur = n;

    while (cur > 2)
    {
        /* Find the smallest L_k = 3·2^k – 1 that is ≥ cur. */
        unsigned __int128 power2 = 1;
        unsigned __int128 L      = 2;          /* L_0 = 2 */
        unsigned __int128 target = (unsigned __int128)cur;

        while (L < target) {
            power2 <<= 1;                     /* 2^k  */
            L = 3 * power2 - 1;               /* L_k  */
        }

        unsigned __int128 Lprev = (L - 1) >> 1;/* L_{k-1} */

        if (target == L) {                    /* n = end of block */
            inc += 1;
            cur = (idx_t)Lprev;
        }
        else if (cur > (idx_t)Lprev) {         /* second copy of block */
            cur -= (idx_t)Lprev;
        }
        /* else cur ≤ Lprev → first copy: keep cur unchanged */
    }

    return base_vals[cur - 1] + inc;
}
