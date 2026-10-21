/*
 * A238329  Fibonacci numbers that have no prime factors of the form 4k+1.
 * https://oeis.org/A238329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fibonacci numbers that have no prime factors of the form 4k+1.
   There are only 11 such Fibonacci numbers, so the sequence stops after
   a(11) = 4807526976.                                 */

seq_t a(idx_t n)
{
    /* Known Fibonacci numbers with the required property.  The list 
       grows only to the 11th element; all further terms are undefined. */
    static const seq_t values[] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)2,
        (seq_t)3,
        (seq_t)8,
        (seq_t)21,
        (seq_t)144,
        (seq_t)987,
        (seq_t)46368,
        (seq_t)2178309,
        (seq_t)4807526976ULL
    };
    idx_t len = sizeof(values) / sizeof(values[0]);

    if (n >= 1 && n <= len) {
        return values[n - 1];
    }
    return 0;   /* undefined for n > 11 */
}
