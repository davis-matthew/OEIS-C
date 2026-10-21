/*
 * A059249  Tersum n + (n-1); write n and n-1 in base 3 and add mod 3 with no
 *          carries.
 * https://oeis.org/A059249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = interpret in base‑3 the per‑digit sum (mod 3) of n and
 *            n‑1
 * Range:     u8 n <= 242, u16 n <= 9242, u32 n <= 9242, u64 n <= 9242,
 *            u128 n <= 9242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n1   = n - 1;          /* n-1 is always defined because n ≥ 1           */
    seq_t result = 0;
    seq_t mult   = 1;           /* 3^k for the current digit position          */
    while (n > 0 || n1 > 0) {
        int d0 = n % 3;          /* least significant digit of n              */
        int d1 = n1 % 3;         /* least significant digit of n‑1            */
        int sum = (d0 + d1) % 3; /* digit of the result, no carry             */
        if (sum) {
            result += (seq_t)sum * mult;
        }
        mult *= 3;               /* advance to next base‑3 power              */
        n   /= 3;
        n1  /= 3;
    }
    return result;
}
