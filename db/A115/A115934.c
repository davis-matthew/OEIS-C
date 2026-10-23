/*
 * A115934  Numbers k such that k^4 contains a pandigital substring.
 * https://oeis.org/A115934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k such that k^4 contains a
 *            pandigital (0‑9) substring in its decimal representation
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --- Helper: test if an __int128 value contains a pandigital substring --- */
static bool has_pandigital_substring(unsigned __int128 v)
{
    /* Maximum digits for k^4 where k <= ~2.5e6: ~27 digits.  */
    char digits[64];         /* hold decimal digits in correct order   */
    size_t len = 0;
    unsigned __int128 tmp = v;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        /* build reverse order first */
        char rev[64];
        size_t rev_len = 0;
        while (tmp != 0) {
            rev[rev_len++] = (char)(tmp % 10);
            tmp /= 10;
        }
        /* reverse into digits[] */
        for (size_t i = 0; i < rev_len; ++i) {
            digits[i] = rev[rev_len - 1 - i];
        }
        len = rev_len;
    }

    if (len < 10) return false;  /* cannot contain 10‑digit substring */

    /* slide window of length 10 */
    for (size_t i = 0; i + 10 <= len; ++i) {
        bool seen[10] = { false };
        bool dup = false;
        int seen_cnt = 0;
        for (size_t j = 0; j < 10; ++j) {
            unsigned d = (unsigned)digits[i + j];
            if (seen[d]) {
                dup = true;
                break;
            }
            seen[d] = true;
            ++seen_cnt;
        }
        if (dup || seen_cnt != 10) continue;  /* not pandigital */
        return true;
    }
    return false;
}

/* --- Cache management for computed sequence values --- */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static uint64_t next_k_vec = 1;
#endif

static seq_t *cache_c = NULL;
static size_t cache_len_c = 0;
static size_t cache_cap_c = 0;
static uint64_t next_k_c = 1;

static void ensure_cache_size(idx_t n)
{
    if (n <= cache_len_c) return;           /* C only path */

    /* C++ path */
#ifdef __cplusplus
    while (cache_vec.size() < (size_t)n) {
        uint64_t k = next_k_vec++;
        unsigned __int128 val = (unsigned __int128)k;
        val *= val;        /* k^2 */
        val *= val;        /* k^4 */
        if (has_pandigital_substring(val)) {
            cache_vec.push_back((seq_t)k);
        }
    }
#else
    /* C path */
    while (cache_len_c < (size_t)n) {
        uint64_t k = next_k_c++;
        unsigned __int128 val = (unsigned __int128)k;
        val *= val;        /* k^2 */
        val *= val;        /* k^4 */
        if (has_pandigital_substring(val)) {
            if (cache_len_c == cache_cap_c) {
                cache_cap_c = (cache_cap_c == 0) ? 16 : cache_cap_c * 2;
                seq_t *newp = (seq_t *)realloc(cache_c, cache_cap_c * sizeof(seq_t));
                if (!newp) abort();
                cache_c = newp;
            }
            cache_c[cache_len_c++] = (seq_t)k;
        }
    }
#endif
}

/* --- Public sequence function --- */
seq_t a(idx_t n)
{
    /* n is 1‑based; n == 0 is undefined in OEIS, return 0 */
    if (n == 0) return (seq_t)0;

#ifdef __cplusplus
    ensure_cache_size(n);
    return cache_vec[(size_t)(n - 1)];
#else
    ensure_cache_size(n);
    return cache_c[(size_t)(n - 1)];
#endif
}
