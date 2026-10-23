/*
 * A010078  Shortest representation of -n in 2's-complement format.
 * https://oeis.org/A010078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^(ceil(log_2(n)+1)) - n (offset 1)
 * Range:     u8 n <= 128, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

static inline idx_t floor_log2_uint64(idx_t n)
{
    /* n ≥ 1; uses GCC/Clang builtin if available, otherwise a simple loop */
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll((unsigned long long)n);
#else
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n)
{
    assert(n >= 1);

    /* compute ceil(log2(n)) */
    idx_t fl = floor_log2_uint64(n);           /* floor */
    idx_t ceil = ((idx_t)1 << fl) == n ? fl : fl + 1;

    /* 2^(ceil(log2(n)+1)) = 1 << (ceil + 1) */
    unsigned bits = (unsigned)(sizeof(seq_t) * 8);
    seq_t result;

    if (ceil + 1 >= bits) {
        /* shift would overflow; use modular arithmetic */
        result = (seq_t)0 - (seq_t)n;          /* 2^bits - n (mod 2^bits) */
    } else {
        result = ((seq_t)1 << (ceil + 1)) - (seq_t)n;
    }
    return result;
}
