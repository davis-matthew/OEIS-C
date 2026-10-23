/*
 * A145911  a(n) = A145909(n)/8.
 * https://oeis.org/A145911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A106619(n) * A106619(n+1) * where * A106619(n) = n if n
 *            is odd and not divisible by 3 * = n/2 if n is even and not
 *            divisible by 3 * = n / (3^e) if n ≡ 0 (mod 3), n odd, * with e =
 *            1 if v₃(n)=1 or 2, else e = 2 * = n / (2·3^e) if n ≡ 0 (mod 3),
 *            n even, * with e as above * (here v₃(n) is the...
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _a106619
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_a106619.h"

seq_t a(idx_t n)
{
    return _a106619(n) * _a106619(n + 1);
}
