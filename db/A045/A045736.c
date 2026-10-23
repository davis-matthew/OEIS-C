/*
 * A045736  Smallest positive integer requiring n syllables to pronounce in
 *          American English.
 * https://oeis.org/A045736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A045736: Smallest positive integer requiring n syllables to pronounce in American English.
 * Formula: a(n+11) = a(n)*1000 + 777, with initial values
 * a(1)=1, a(2)=7, a(3)=11, a(4)=27, a(5)=77, a(6)=111,
 * a(7)=127, a(8)=177, a(9)=777, a(10)=1127, a(11)=1177.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 safely */

    /* initial 11 terms */
    static const seq_t base[11] = {
        (seq_t)1,   (seq_t)7,   (seq_t)11,   (seq_t)27,
        (seq_t)77,  (seq_t)111, (seq_t)127,  (seq_t)177,
        (seq_t)777, (seq_t)1127, (seq_t)1177
    };

    /* locate which block of 11 the index falls into */
    idx_t k     = (n - 1) % 11;    /* 0 .. 10 */
    idx_t block = (n - 1) / 11;    /* how many times to apply the recurrence */

    seq_t result = base[k];

    for (idx_t i = 0; i < block; ++i) {
        result = result * (seq_t)1000 + (seq_t)777;
    }

    return result;
}
