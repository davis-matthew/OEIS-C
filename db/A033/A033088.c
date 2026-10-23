/*
 * A033088  Numbers in which all pairs of consecutive digits differ by 2.
 * https://oeis.org/A033088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 149, u32 n <= 947, u64 n <= 947,
 *            u128 n <= 947, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033088:  Numbers in which all pairs of consecutive digits differ by 2.  
   The function generates all admissible numbers by depth‑first enumeration
   of digit strings of increasing length and caches them. */

#include <stdint.h>
#include <stddef.h>

/* maximum number of terms we expect to store (good for n up to ~10000) */
#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t next_length = 1;

/* add a value to the cache, if space permits */
static void cache_add(seq_t val)
{
    if (cache_len < MAX_CACHE) {
        cache[cache_len++] = val;
    }
}

/* depth‑first generate numbers of target_len digits ending with last_digit. */
static void dfs_gen(idx_t target_len, idx_t cur_len, seq_t val, uint8_t last_digit)
{
    if (cur_len == target_len) {
        cache_add(val);
        return;
    }
    if (last_digit >= 2) {
        dfs_gen(target_len, cur_len + 1, val * 10 + (last_digit - 2), last_digit - 2);
    }
    if (last_digit <= 7) {                    /* last_digit + 2 <= 9 */
        dfs_gen(target_len, cur_len + 1, val * 10 + (last_digit + 2), last_digit + 2);
    }
}

/* generate terms until the cache contains at least n terms */
static void ensure_until(idx_t n)
{
    while (cache_len < n) {
        if (next_length == 1) {                /* single digit numbers 1..9 */
            for (uint8_t d = 1; d <= 9; ++d) {
                cache_add((seq_t)d);
            }
            next_length = 2;
        } else {                                /* longer numbers */
            for (uint8_t d = 1; d <= 9; ++d) {
                dfs_gen(next_length, 1, (seq_t)d, d);
            }
            ++next_length;
        }
    }
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* outside the defined offset – undefined */
    ensure_until(n);
    return cache[n - 1];
}
