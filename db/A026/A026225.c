/*
 * A026225  Numbers of the form 3^i * (3k+1).
 * https://oeis.org/A026225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A026225
 * a(n) = nth positive integer whose least non‑zero digit in base‑3 is 1.
 * (Equivalently, n = 3^i * (3k+1) for some i ≥ 0, k ≥ 0.)
 * Implementation: brute–force scan of natural numbers, testing the
 * required property.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by sequence offset; nothing else is required. */
    seq_t x = 1;          /* current integer being tested          */
    idx_t count = 0;      /* number of valid terms found so far   */

    while (1) {
        seq_t t = x;
        /* strip all trailing powers of 3    */
        while (t % 3 == 0) {
            t /= 3;
        }
        /* now t ≡ 1 (mod 3) ⇔ x has the form 3^i·(3k+1)      */
        if (t % 3 == 1) {
            ++count;
            if (count == n) {
                return x;  /* found the nth term                  */
            }
        }
        ++x;              /* move to the next integer            */
    }
}
