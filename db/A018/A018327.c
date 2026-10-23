/*
 * A018327  Divisors of 190.
 * https://oeis.org/A018327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018327: Divisors of 190 – a(n) is the n‑th smallest divisor of 190 */

seq_t a(idx_t n)
{
    /* Enumerate all subsets of the prime factorization 190 = 2 * 5 * 19.
       Masks 0..7 produce the divisors in ascending order: 1,2,5,10,19,38,95,190. */
    static seq_t divs[8];
    static int init = 0;
    if (!init) {
        const idx_t primes[3] = {2, 5, 19};
        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t val = 1;
            for (idx_t j = 0; j < 3; ++j) {
                if (mask & ((idx_t)1 << j))
                    val *= (seq_t)primes[j];
            }
            divs[mask] = val;
        }
        init = 1;
    }
    return (n >= 1 && n <= 8) ? divs[n - 1] : 0;
}
