/*
 * A055125  Base-15 complement of n (write n in base 15, then replace each
 *          digit with its base-15 negative).
 * https://oeis.org/A055125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = base‑15 complement of n (each base‑15 digit d is replaced
 *            by (15–d) mod 15)
 * Range:     u8 n <= 224, u16 n <= 3374, u32 n <= 3374, u64 n <= 3374,
 *            u128 n <= 3374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pw  = 1;
    idx_t t = n;
    while (t > 0) {
        unsigned d = (unsigned)(t % 15);
        unsigned cd = (d == 0) ? 0 : 15 - d;   /* (15-d) mod 15 */
        res += (seq_t)cd * pw;
        t /= 15;
        pw *= 15;
    }
    return res;
}
