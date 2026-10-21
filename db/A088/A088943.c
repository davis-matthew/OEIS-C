/*
 * A088943  a(n)=60*sum(1<=i<=j<=k<=n,i*j^2/k).
 * https://oeis.org/A088943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60 * ∑_{1≤i≤j≤k≤n} i·j²/k Simplified to the polynomial:
 *            a(n) = ½ * Σ_{k=1}^{n} (12k⁴ + 45k³ + 50k² + 15k – 2) = (12·S4 +
 *            45·S3 + 50·S2 + 15·S1 – 2n) / 2 where S1 = Σk, S2 = Σk², S3 =
 *            Σk³, S4 = Σk⁴
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // Use 128‑bit intermediate arithmetic to avoid overflow
    __int128 nn = n;

    // sums of powers of k
    __int128 S1 = nn * (nn + 1) / 2;                      // Σk
    __int128 S2 = nn * (nn + 1) * (2 * nn + 1) / 6;       // Σk²
    __int128 S3 = S1 * S1;                                 // (Σk)² = Σk³
    __int128 S4 = nn * (nn + 1) * (2 * nn + 1) *
                  (3 * nn * nn + 3 * nn - 1) / 30;          // Σk⁴

    __int128 total = 12 * S4 + 45 * S3 + 50 * S2 + 15 * S1 - 2 * nn;
    __int128 result = total / 2;      // division is exact

    return (seq_t)result;
}
