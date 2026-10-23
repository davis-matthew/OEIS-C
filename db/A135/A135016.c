/*
 * A135016  Numbers n with property that for each single digit d of n, we can
 *          also see the decimal expansion of 2^d as a substring of n. Also n
 *          may not contain any zero digits.
 * https://oeis.org/A135016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 300, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A135016 – Numbers n with property that for each digit d of n
 * we can also see the decimal expansion of 2^d as a substring of n.
 * Each n must not contain any zero digits.
 *
 * Computation method:
 *   Enumerate all positive integers written in base‑10 that contain only
 *   digits 1…9, in increasing numeric order.  For each candidate string
 *   s, check the property above.  If the candidate is valid, convert it
 *   to a seq_t value and append it to a static cache.  The cache is
 *   grown dynamically with malloc/realloc as more terms are required.
 *
 *   This algorithm runs in O(T·L) time where T is the number of terms
 *   generated and L is the average number of digits of those terms,
 *   which is easily fast enough for n ≤ 10 000.
 *
 *   The code is fully portable to both C11 and C++17, does not rely on
 *   any non‑portable extensions, and uses only the operations allowed
 *   for seq_t/idx_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External types supplied by the build system */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Pre‑computed strings for 2^d, for d = 1 … 9. */
static const char *const pow2_str[10] = {
    NULL,        /* 0 unused */
    "2",         /* 2^1  */
    "4",         /* 2^2  */
    "8",         /* 2^3  */
    "16",        /* 2^4  */
    "32",        /* 2^5  */
    "64",        /* 2^6  */
    "128",       /* 2^7  */
    "256",       /* 2^8  */
    "512"        /* 2^9  */
};

/* Cache of generated terms */
static seq_t *cache      = NULL;   /* array of seq_t values   */
static size_t cache_cap  = 0;      /* currently allocated size*/
static size_t cache_size = 0;      /* number of terms stored  */

/* State for enumerating numbers with digits 1…9 */
static int  curr_len  = 1;     /* current number of digits      */
static uint64_t curr_idx = 0;  /* index within this length (0‑based) */

/* Pre‑computed powers of 9: pow9[i] = 9^i, up to the first overflow */
static uint64_t pow9[65];
static bool pow9_inited = false;

static void
init_pow9(void)
{
    if (pow9_inited) return;
    pow9[0] = 1;
    for (int i = 1; i < 65; ++i) {
        if (pow9[i - 1] > UINT64_MAX / 9) {
            pow9[i] = 0;            /* mark overflow; will not be used */
            break;
        }
        pow9[i] = pow9[i - 1] * 9;
    }
    pow9_inited = true;
}

/* Check whether the candidate string s (length len) satisfies the property. */
static bool
is_valid(const char *s, int len)
{
    bool present[10] = {false};
    for (int i = 0; i < len; ++i)
        present[(int)(s[i] - '0')] = true;

    /* For every present digit d, decide whether pow2_str[d] occurs as substring */
    for (int d = 1; d <= 9; ++d) {
        if (!present[d]) continue;
        if (strstr(s, pow2_str[d]) == NULL)
            return false;
    }
    return true;
}

/* Ensure that at least n_needed terms are generated and cached. */
static void
generate_until(idx_t n_needed)
{
    if (cache_size >= (size_t)n_needed) return;

    init_pow9();

    /* Temporary buffer to construct the candidate digits as a string. */
    /* Max length we ever need is 64 (within uint64_t limits). */
    static char tmp[128];

    while (cache_size < (size_t)n_needed) {
        /* Build the decimal string for the current (curr_len, curr_idx). */
        uint64_t idx = curr_idx;
        for (int pos = curr_len - 1; pos >= 0; --pos) {
            int digit = idx % 9;       /* 0…8 maps to '1'…'9' */
            idx /= 9;
            tmp[pos] = '1' + digit;
        }
        tmp[curr_len] = '\0';

        if (is_valid(tmp, curr_len)) {
            /* Convert the string to a seq_t value. */
            seq_t val = 0;
            for (int i = 0; i < curr_len; ++i)
                val = val * 10 + (seq_t)(tmp[i] - '0');

            /* Grow the cache if needed. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* In the unlikely event of allocation failure, abort. */
                    abort();
                }
                cache      = new_cache;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = val;
        }

        /* Advance to the next candidate. */
        ++curr_idx;
        if (curr_idx >= pow9[curr_len]) {
            ++curr_len;
            curr_idx = 0;
        }
    }
}

/* Required function entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* a(0) is undefined, but guard just in case */
    generate_until(n);
    return cache[n - 1];
}
