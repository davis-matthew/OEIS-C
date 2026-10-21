/*
 * A060582  If the final digit of n in base 3 is the same as a([n/3]) then
 *          this digit, otherwise a(n)= mod 3-sum of these two digits, with
 *          a(0)=0.
 * https://oeis.org/A060582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (-a(floor(n/3)) - n) mod 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is, a(n) equals the ternary digit of n if it equals a(n/3);
   otherwise it equals the sum of the two digits modulo 3 (the unique
   value that makes a(n)+a(n/3)+n ≡ 0 (mod 3).  a(0)=0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prev = a(n / 3);       /* a(floor(n/3)) */
    seq_t d    = n % 3;          /* last base‑3 digit of n */

    /* compute (3 - (prev + d) mod 3) mod 3, which is equivalent to
       (-prev - d) mod 3 and works for both equal and different digits. */
    seq_t sum = (prev + d) % 3;
    return (3 - sum) % 3;
}
