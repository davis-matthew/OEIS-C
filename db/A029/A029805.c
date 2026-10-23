/*
 * A029805  Numbers k such that k^2 is palindromic in base 8.
 * https://oeis.org/A029805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   grow_seq_array, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029805: numbers k such that k² is palindromic in base 8 (search). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/grow_seq_array.h"

static inline bool is_palindrome_uint128(unsigned __int128 x, idx_t base)
{
    unsigned char digits[64];
    int len = 0;
    if (x == 0) return true;
    while (x) {
        digits[len++] = (unsigned char)(x % base);
        x /= base;
    }
    for (int i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i])
            return false;
    return true;
}

static seq_t *cache = NULL;     /* array of computed k values             */
static size_t cap   = 0;        /* current capacity of 'cache'           */
static size_t len   = 0;        /* number of terms stored                */
static idx_t  next  = 0;        /* next candidate k to test              */

static void grow_cache(size_t need)
{
    if (need <= cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    /* grow_seq_array reallocates the array appropriately for seq_t. */
    grow_seq_array(&cache, &cap, new_cap);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for undefined offset          */

    if (n <= len)                   /* already computed                     */
        return cache[n - 1];

    while (len < n) {
        seq_t k = (seq_t)next;
        bool ok;
        if (sizeof(seq_t) <= 8) {   /* small builtin types: use 128‑bit prod. */
            unsigned __int128 sq = (unsigned __int128)next * next;
            ok = is_palindrome_uint128(sq, 8);
        } else {                   /* seq_t is at least 16 bytes (big int).  */
            seq_t sq = k * k;
            ok = is_palindrome(sq, 8);
        }
        if (ok) {                   /* store term                          */
            grow_cache(len + 1);
            cache[len++] = k;
        }
        ++next;
        if (next == 0) break;        /* overflow guard                       */
    }

    return (len >= n) ? cache[n - 1] : (seq_t)0;
}
