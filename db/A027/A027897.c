/*
 * A027897  Divisors of 10^12 - 1.
 * https://oeis.org/A027897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 178, u64 n <= 256, u128 n <= 256,
 *            big any n
 * Helpers:   _cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 10^12 − 1, sorted in ascending order. */

#include <stdlib.h>
#include "../utils/_cmp_seq.h"

/* Prime factors of 10^12 − 1:
   3^3 · 7 · 11 · 13 · 37 · 101 · 9901
   The number of divisors is 4·2⁶ = 256. */
static const seq_t _primes[7] = {3, 7, 11, 13, 37, 101, 9901};
static const unsigned char _exponents[7] = {3, 1, 1, 1, 1, 1, 1};

/* Recursive generation of all divisors. */
static void _gen_divisors(int idx, seq_t cur, seq_t *out, int *cnt)
{
    if (idx == 7) {
        out[(*cnt)++] = cur;
        return;
    }
    seq_t mult = 1;
    for (unsigned e = 0; e <= _exponents[idx]; ++e) {
        _gen_divisors(idx + 1, cur * mult, out, cnt);
        mult = mult * _primes[idx];
    }
}

/* Return the n-th divisor of 10^12‑1 (n ≥ 1).  If n > 256, return 0. */
seq_t a(idx_t n)
{
    static seq_t _div[256];
    static int _init = 0;

    if (!_init) {
        int cnt = 0;
        _gen_divisors(0, 1, _div, &cnt);        /* cnt should be 256 */
        qsort(_div, cnt, sizeof(seq_t), _cmp_seq);
        _init = 1;
    }

    if (n == 0)          /* safety, but n ≥ 1 in the spec */
        return 0;
    if (n > 256)         /* out of bounds – no further divisors */
        return 0;

    return _div[n - 1];
}
