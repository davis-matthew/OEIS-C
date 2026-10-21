/*
 * A231664  a(n) = Sum_{i=0..n} digsum_4(i), where digsum_4(i) = A053737(i).
 * https://oeis.org/A231664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_4(i), where digsum_4(i) is the sum of
 *            the base‑4 digits of i
 * Range:     u8 n <= 58, u16 n <= 7131, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t pow   = 1;                     /* 4^j */

    while (pow <= n)                     /* iterate over digit positions */
    {
        /* contributions from this digit position j */
        idx_t higher  = (n / pow) / 4;    /* number of full 4‑digit blocks above */
        idx_t current = (n / pow) % 4;    /* digit value at position j */
        idx_t lower   = n % pow;          /* lower (j − 1)-digit part */

        seq_t term = (seq_t)higher * 6 * (seq_t)pow;            /* full cycles */
        term += (seq_t)current * (current - 1) / 2 * (seq_t)pow; /* partial full cycle */
        term += (seq_t)current * ((seq_t)lower + 1);            /* remaining part */

        total += term;

        /* move to next power of 4, guarding against overflow */
        if (pow > n / 4) {
            break;
        }
        pow *= 4;
    }

    return total;
}
