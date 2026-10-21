/*
 * A070582  a(n) = n^4 mod 39.
 * https://oeis.org/A070582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 39
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 39 first to avoid overflow when
       multiplying.  The intermediate multiplication uses 64‑bit
       unsigned arithmetic which is safe for all valid inputs. */
    const idx_t M = 39;
    idx_t nmod = n % M;
    unsigned long long res = 1ULL;
    unsigned long long base = (unsigned long long)nmod;

    for (int i = 0; i < 4; ++i)
        res = (res * base) % M;

    return (seq_t)res;
}
