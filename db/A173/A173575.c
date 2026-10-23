/*
 * A173575  5-Factorions: equal to the sum of the quintuple factorials of
 *          their digits in base 10.
 * https://oeis.org/A173575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 5‑factorions: a(n) is the n‑th integer that equals the sum of the quintuple
   factorials of its decimal digits.  The sequence is finite (only 7 terms).  */

#include <stddef.h>   /* for size_t */

/* Quintuple factorials of digits 0–9. */
static const seq_t _qfact[10] = {
    0ull, 1ull, 2ull, 3ull, 4ull,
    5ull, 6ull, 14ull, 24ull, 36ull
};

/* Cache of found 5‑factorions.  The sequence has at most 7 terms, so
   we only need to search up to 999. */
static seq_t _cache[1000];
static size_t _cache_len = 0;

/* Initialise the cache once. */
static void _init_cache(void)
{
    if (_cache_len != 0) return;      /* already built */

    for (idx_t n = 1; n < 1000; ++n) {
        seq_t sum = 0ull;
        idx_t x   = n;
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            sum += _qfact[d];
            x   /= 10;
        }
        if (sum == (seq_t)n)
            _cache[(_cache_len)++] = (seq_t)n;
    }
}

seq_t a(idx_t n)
{
    _init_cache();
    if ((size_t)n == 0) return 0ull;                    /* undefined for n = 0 */
    if ((size_t)n <= _cache_len) return _cache[(size_t)n - 1];
    return 0ull;                                       /* sequence terminates after 7 terms */
}
