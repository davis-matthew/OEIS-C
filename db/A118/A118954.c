/*
 * A118954  Numbers that cannot be written as 2^k + prime.
 * https://oeis.org/A118954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that cannot be written as 2^k + prime (OEIS
 *            A118954)
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *seq_cache = NULL;      /* cached non‑representable numbers */
static size_t seq_len = 0;           /* number of terms stored */
static size_t seq_cap = 0;           /* current array capacity */
static idx_t last_checked = 0;       /* last integer examined */

/* Append a new term to the cache, enlarging the array if necessary */
static void append_cache(seq_t val) {
    if (seq_len == seq_cap) {
        /* Ensure capacity for at least `seq_len + 1` elements. */
        grow_seq(&seq_cache, &seq_cap, seq_len + 1);
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1; behaviour for n == 0 is undefined. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Next integer to test */
        last_checked++;
        seq_t m = (seq_t)last_checked;

        /* Check if m can be expressed as 2^k + prime. */
        bool representable = false;
        seq_t power = (seq_t)1;              /* 2^0 */
        while (power <= m) {
            seq_t p = m - power;
            if (p >= (seq_t)2 && is_prime_big(p)) {
                representable = true;
                break;
            }
            power <<= 1;                     /* next power of two */
        }

        /* If not representable, it belongs to the sequence. */
        if (!representable) {
            append_cache(m);
        }
    }

    /* Return the (n-1)th term (indices start at 1). */
    return seq_cache[n - 1];
}
