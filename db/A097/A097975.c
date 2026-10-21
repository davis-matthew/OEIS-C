/*
 * A097975  a(n) is the prime divisor of n which is >= sqrt(n), or 0 if there
 *          is no such prime divisor.
 * https://oeis.org/A097975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the prime divisor of n that is >= sqrt(n); if no such
 *            divisor exists, a(n) = 0
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t definition (8,16,32,64,128 bits or big-int).
   It computes the largest prime factor of n and checks whether it is at least
   the integer square root of n.                                     */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the answer is 0. */
    if (n < 2)
        return 0;

    /* Work with 64‑bit arithmetic; n fits in idx_t (uint64_t). */
    uint64_t nn   = (uint64_t)n;   /* original value of n      */
    uint64_t tmp  = nn;           /* temporary copy to factor */
    uint64_t largest = 0;         /* largest prime factor seen */

    /* Handle factor 2 separately. */
    if (tmp % 2 == 0) {
        largest = 2;
        while (tmp % 2 == 0)
            tmp /= 2;
    }

    /* Odd factors up to sqrt(tmp). */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            if (p > largest)
                largest = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* Remaining tmp > 1 is a prime factor > sqrt(original n). */
    if (tmp > 1) {
        if (tmp > largest)
            largest = tmp;
    }

    /* Determine if the largest prime factor is >= sqrt(n).
       We use 128-bit arithmetic to avoid overflow. */
    unsigned __int128 largest_sq = (unsigned __int128)largest * largest;
    if (largest_sq >= (unsigned __int128)nn)
        return (seq_t)largest;   /* fits in seq_t by definition */
    else
        return 0;
}
