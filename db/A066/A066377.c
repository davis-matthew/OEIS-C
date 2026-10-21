/*
 * A066377  Sum of the first n numbers k such that floor(sqrt(k)) divides k.
 * https://oeis.org/A066377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let q = ⌊(n-1)/3⌋, r = (n-1) mod 3 a(n) = q·(q+1)·(q+2) + {
 *            (q+1)² if r==0 (q+1)² + (q+1)(q+2) if r==1 3·(q+1)(q+2) if r==2
 *            }
 * Range:     u8 n <= 16, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;

    seq_t base = (seq_t)q * (seq_t)(q + 1);
    base *= (seq_t)(q + 2);

    seq_t result = base;
    seq_t mp1 = (seq_t)(q + 1);

    if (r == 0) {
        result += mp1 * mp1;
    } else if (r == 1) {
        result += mp1 * mp1 + mp1 * (mp1 + 1);
    } else { // r == 2
        result += (seq_t)3 * mp1 * (mp1 + 1);
    }

    return result;
}
