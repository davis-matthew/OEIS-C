/*
 * A081062  Neither 3-smooth numbers nor prime powers.
 * https://oeis.org/A081062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer that is composite, * not
 *            3‑smooth (i.e. not of the form 2^a·3^b), and not a prime power.
 *            * The function generates candidates in increasing order and *
 *            skips those that fail any of the three conditions
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n>=1 in use */

    idx_t count = 0;                /* how many valid terms found */
    idx_t num   = 2;                /* current candidate integer   */

    while (1)
    {
        /* Skip small primes directly */
        if (num <= 3) { num++; continue; }

        /* --- 3‑smooth test: only admissible if after removing all 2’s and 3’s, the rest is 1 --- */
        idx_t tmp = num;
        while ((tmp % 2) == 0) tmp /= 2;
        while ((tmp % 3) == 0) tmp /= 3;
        if (tmp == 1) { num++; continue; }   /* num is 3‑smooth, skip */

        /* --- find smallest prime factor (trial division) --- */
        idx_t spf = 0;
        for (idx_t d = 2; d * d <= num; ++d)
        {
            if (num % d == 0) { spf = d; break; }
        }

        if (spf == 0) {                 /* num is prime, skip */
            num++; continue;
        }

        /* --- check for prime‑power: divide by spf repeatedly --- */
        idx_t rem = num;
        while (rem % spf == 0) rem /= spf;
        if (rem == 1) { num++; continue; }   /* num is a prime power, skip */

        /* num passes all tests: it's a valid sequence term */
        ++count;
        if (count == n) return (seq_t)num;
        ++num;          /* move to next candidate */
    }
}
