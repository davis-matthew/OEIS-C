/*
 * A144095  a(n) = number of exponents of the prime-factorization of n that
 *          occur somewhere in n when the exponents and n are represented in
 *          base 2.
 * https://oeis.org/A144095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2471, u16 n <= 8738, u32 n <= 8738, u64 n <= 8738,
 *            u128 n <= 8738, big any n
 * Helpers:   bitlen_seq_t, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of exponents e in the prime factorization of n such
 * that the binary representation of e occurs as a contiguous substring
 * in the binary representation of n (all numbers are ≥ 1, offset 1).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* helper functions provided by the build system */
#include "../utils/factor_u64.h"
#include "../utils/bitlen_seq_t.h"

/* ------------------------------------------------------------------ */
/* Convert an unsigned 64‑bit integer 'x' to its binary string of length
 * 'len' (most significant bit first).  The caller supplies a buffer
 * at least len+1 characters long.  NULL terminator is added.          */
static void uint64_to_bin(uint64_t x, size_t len, char *buf)
{
    for (size_t i = 0; i < len; ++i) {
        buf[i] = ((x >> (len - 1 - i)) & 1ULL) ? '1' : '0';
    }
    buf[len] = '\0';
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)                     /* a(1) = 0 */
        return 0;

    /* factorization: primes[ ], exps[ ] hold the factor and exponent */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* binary representation of n */
    size_t len_n = bitlen_seq_t((seq_t)n);
    char bin_n[65];                /* 64 bits max + '\0' */
    uint64_to_bin((uint64_t)n, len_n, bin_n);

    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        size_t len_e = bitlen_seq_t((seq_t)e);
        char bin_e[65];
        uint64_to_bin(e, len_e, bin_e);

        if (strstr(bin_n, bin_e) != NULL)
            result = result + 1;   /* count this exponent */
    }

    return result;
}
