/*
 * A084050  Numbers n such that at least one permutation of the digits of n
 *          yields a palindrome.
 * https://oeis.org/A084050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate numbers in increasing order and return the
 * nth element of sequence A084050.  A number n is accepted if
 * some permutation of its decimal digits (zeros allowed at the front
 * and then dropped) yields a palindrome.  Equivalently, after
 * removing an arbitrary number of leading zeros, the remaining
 * digits can be rearranged to form a palindrome, i.e. at most
 * one digit has an odd occurrence. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether n can be permuted to produce a palindrome
 * after stripping leading zeros.  The rule is:
 *   * Count digits of n.
 *   * Let z = number of zeros.
 *   * Let odd = number of digits with odd count.
 *   * If z == 0, accept if odd ≤ 1.
 *   * If z > 0, we may either keep the zero parity as is
 *     (remove an even number of zeros) or toggle it
 *     (remove an odd number).  Accept if either
 *     odd ≤ 1 or odd toggled ≤ 1. */
static bool is_permutable_to_palindrome(seq_t n) {
    unsigned int cnt[10] = {0};
    seq_t x = n;

    /* Count digits. */
    while (x > (seq_t)0) {
        unsigned int d = (unsigned)(x % (seq_t)10);
        cnt[d] = cnt[d] + 1;
        x = x / (seq_t)10;
    }

    unsigned int zero_cnt = cnt[0];
    unsigned int odd = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i] & 1U)
            odd += 1U;

    if (zero_cnt == 0)
        return odd <= 1U;

    /* Keep zero parity unchanged (remove even number of zeros). */
    if (odd <= 1U)
        return true;

    /* Toggle zero parity (remove odd number of zeros). */
    unsigned int odd_toggled = odd;
    if (zero_cnt & 1U)
        odd_toggled -= 1U;  /* zero count was odd -> becomes even */
    else
        odd_toggled += 1U;  /* zero count was even -> becomes odd */

    return odd_toggled <= 1U;
}

/* a(n): nth term of A084050 (1‑based).  The function lazily
 * computes terms and caches them to avoid recomputation on
 * subsequent calls. */
seq_t a(idx_t n) {
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms stored */
    static idx_t cache_cap  = 0;    /* allocated capacity */
    static seq_t last_checked = (seq_t)0;  /* last integer examined */

    /* Ensure cache has capacity for at least n terms. */
    while (cache_size < n) {
        /* Advance to next integer. */
        ++last_checked;

        if (is_permutable_to_palindrome(last_checked)) {
            /* Store the term. */
            if (cache_size == cache_cap) {
                /* Grow cache (double). */
                size_t new_cap = (cache_cap == 0) ? 128 : (size_t)cache_cap * 2;
                seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
                if (!new_buf) {
                    /* Allocation failure – abort with zero. */
                    return (seq_t)0;
                }
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}
