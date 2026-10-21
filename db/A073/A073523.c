/*
 * A073523  The set of 36 consecutive primes that form a 6 X 6 pandiagonal
 *          magic square with the smallest magic constant (930).
 * https://oeis.org/A073523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A073523 – 36 consecutive primes that form a 6×6 pandiagonal magic
   square with magic constant 930.
   The sequence is the list of these primes in increasing order, starting
   with 67 and ending with 251. */
/* Terms: 67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,
          167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251 */
seq_t a(idx_t n)
{
    /* 36 consecutive primes from 67 to 251, inclusive.  */
    static const seq_t primes[36] = {
        67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
        109,113,127,131,137,139,149,151,157,163,
        167,173,179,181,191,193,197,199,211,223,
        227,229,233,239,241,251
    };

    if (n < 1 || n > 36)
        return 0;          /* undefined for indices outside 1 – 36 */

    return primes[n - 1];
}
