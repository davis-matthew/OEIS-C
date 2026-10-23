/*
 * A152136  a(0) = 0; thereafter a(n) is always the smallest integer > a(n-1)
 *          not leading to a contradiction, such that the concatenation of any
 *          two consecutive digits in the sequence is a prime.
 * https://oeis.org/A152136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   div10, mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A152136: a(0)=0; for n>0, a(n) is the smallest integer > a(n‑1)
   such that when the digits of all terms up to a(n) are concatenated,
   every pair of consecutive digits forms a prime.  This implementation
   builds the sequence on demand, memoising it for repeated calls.   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/div10.h"
#include "../utils/mod10.h"

/* --------------------------------------------------------------------
   Precomputation: adjacency lists for digits where concatenation is prime
   Also compute a reachability table poss[d][len] that tells whether a
   path of length len can start at digit d (len=no digits remaining after
   the chosen digit).  The table is filled lazily up to the needed
   length. -------------------------------------------------------------*/
#define MAX_DIGITS 40000   /* enough for n≤10000 */
static bool prime_table[100];
static int  next_digits[10][10];
static int  next_count[10];

static bool poss[10][MAX_DIGITS + 1];
static size_t memo_len = 0;   /* currently computed maximum length */

/* initialise prime_table and adjacency lists */
static void init_prime_and_adjacency(void)
{
    /* prime[0..99] */
    for (int i = 0; i < 100; ++i) {
        bool isP = false;
        if (i >= 2) {
            isP = true;
            for (int d = 2; d * d <= i; ++d)
                if (i % d == 0) { isP = false; break; }
        }
        prime_table[i] = isP;
    }

    /* adjacency */
    for (int d = 0; d < 10; ++d) {
        int cnt = 0;
        for (int e = 0; e < 10; ++e)
            if (prime_table[d * 10 + e])
                next_digits[d][cnt++] = e;
        next_count[d] = cnt;
    }

    /* base case for reachability */
    for (int d = 0; d < 10; ++d)
        poss[d][0] = true;   /* zero digits left: always possible */
    memo_len = 0;
}

/* extend reachability table up to length new_len (inclusive) */
static void ensure_memo(size_t new_len)
{
    if (new_len <= memo_len) return;

    for (size_t len = memo_len + 1; len <= new_len; ++len) {
        for (int d = 0; d < 10; ++d) {
            bool ok = false;
            for (int k = 0; k < next_count[d]; ++k) {
                int e = next_digits[d][k];
                if (poss[e][len - 1]) { ok = true; break; }
            }
            poss[d][len] = ok;
        }
    }
    memo_len = new_len;
}

/* --------------------------------------------------------------------
   Global buffers used during digit extraction / DFS                       */
static int  lower_digits[MAX_DIGITS];
static int  lower_len;
static int  curr_len;
static int  allowed_digits[10];
static int  allowed_cnt;

/* digits of a number in most‑significant‑first order */
static void digits_of(seq_t x, int *buf, size_t *len)
{
    seq_t v = x;
    if (v == 0) { buf[0] = 0; *len = 1; return; }
    size_t pos = 0;
    while (v > (seq_t)0) {
        buf[pos++] = (int)(v % (seq_t)10);
        v /= (seq_t)10;
    }
    /* reverse to MSB first */
    for (size_t i = 0; i < pos / 2; ++i) {
        int t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    *len = pos;
}

/* --------------------------------------------------------------------
   DFS for minimal number of a given length that is ≥ lower bound.
   tight= true means prefix so far equals lower prefix.          */
static bool dfs_ge(int pos, bool tight, int prev_digit)
{
    if (pos == curr_len) return true;          /* built all digits  */

    if (pos == 0) {   /* first digit */
        for (int i = 0; i < allowed_cnt; ++i) {
            int d = allowed_digits[i];
            if (tight && d < lower_digits[0]) continue;
            bool tight_next = tight && (d == lower_digits[0]);
            if (!poss[d][curr_len - 1]) continue;
            if (dfs_ge(pos + 1, tight_next, d)) {
                lower_digits[pos] = d;   /* store chosen digit */
                return true;
            }
        }
    } else {          /* subsequent digits */
        int prev = prev_digit;
        for (int k = 0; k < next_count[prev]; ++k) {
            int d = next_digits[prev][k];
            if (tight && d < lower_digits[pos]) continue;
            bool tight_next = tight && (d == lower_digits[pos]);
            if (!poss[d][curr_len - pos - 1]) continue;
            if (dfs_ge(pos + 1, tight_next, d)) {
                lower_digits[pos] = d;
                return true;
            }
        }
    }
    return false;   /* no digit leads to solution  */
}

/* build smallest valid number of length len without lower bound   */
static bool build_min_no_bound(int len)
{
    if (len == 0) return false;
    /* first digit */
    int first = -1;
    for (int i = 0; i < allowed_cnt; ++i) {
        int d = allowed_digits[i];
        if (poss[d][len - 1]) { first = d; break; }
    }
    if (first < 0) return false;
    lower_digits[0] = first;
    int prev = first;
    for (int pos = 1; pos < len; ++pos) {
        int chosen = -1;
        for (int k = 0; k < next_count[prev]; ++k) {
            int d = next_digits[prev][k];
            if (poss[d][len - pos - 1]) { chosen = d; break; }
        }
        if (chosen < 0) return false;
        lower_digits[pos] = chosen;
        prev = chosen;
    }
    return true;
}

/* --------------------------------------------------------------------
   Find the next term in the sequence given the previous term and its
   last digit.  This search tries numbers of increasing length until
   a valid one is found.  The resulting value is returned.              */
static seq_t find_next(seq_t prev_val, int last_digit_prev)
{
    seq_t lower = prev_val + (seq_t)1;   /* minimal candidate */

    /* digits of lower bound (MSB→LSB) */
    uint8_t temp[MAX_DIGITS];
    size_t tlen = 0;
    if (lower == (seq_t)0) { temp[0] = 0; tlen = 1; }
    else {
        seq_t v = lower;
        while (v > (seq_t)0) {
            temp[tlen++] = (uint8_t)(v % (seq_t)10);
            v /= (seq_t)10;
        }
        for (size_t i = 0; i < tlen / 2; ++i) {
            uint8_t t = temp[i];
            temp[i] = temp[tlen - 1 - i];
            temp[tlen - 1 - i] = t;
        }
    }
    lower_len = tlen;
    for (size_t i = 0; i < tlen; ++i) lower_digits[i] = temp[i];

    /* determine allowed first digits based on adjacency to last_digit_prev */
    allowed_cnt = 0;
    for (int d = 0; d < 10; ++d) {
        if (prime_table[last_digit_prev * 10 + d]) {
            allowed_digits[allowed_cnt++] = d;
        }
    }
    if (allowed_cnt == 0) return lower;   /* should not happen */

    int try_len = lower_len;
    while (try_len <= MAX_DIGITS) {
        ensure_memo((size_t)try_len - 1);
        bool found = false;
        if (try_len == lower_len) {
            curr_len = try_len;
            found = dfs_ge(0, true, -1);
        } else {
            curr_len = try_len;
            found = build_min_no_bound(try_len);
        }
        if (found) {
            seq_t result = 0;
            for (int i = 0; i < try_len; ++i)
                result = result * (seq_t)10 + (seq_t)lower_digits[i];
            return result;
        }
        ++try_len;
    }
    /* should never reach here */
    return lower;
}

/* --------------------------------------------------------------------
   Sequence cache (memoisation) -------------------------------------- */
static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static size_t cache_size = 0;
static size_t last_computed = 0;
static bool init_done = false;

static void init_once(void)
{
    if (init_done) return;
    init_prime_and_adjacency();

    size_t init_cap = 10240;           /* safety margin */
    cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
    computed = (unsigned char*)malloc(init_cap * sizeof(unsigned char));
    for (size_t i = 0; i < init_cap; ++i) computed[i] = 0;

    cache[0] = 0;
    computed[0] = 1;
    last_computed = 0;
    init_done = true;
}

/* --------------------------------------------------------------------
   Public function required by the judge -------------------------------- */
seq_t a(idx_t n)
{
    init_once();

    /* enlarge cache when necessary */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_size ? cache_size : 1;
        while ((size_t)n >= new_cap) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        computed = (unsigned char*)realloc(computed, new_cap * sizeof(unsigned char));
        for (size_t i = cache_size; i < new_cap; ++i) computed[i] = 0;
        cache_size = new_cap;
    }

    /* compute all terms up to n if they are missing */
    for (idx_t i = last_computed + 1; (size_t)i <= n; ++i) {
        if (i == 0) continue;           /* already initialised */
        seq_t prev   = cache[i - 1];
        int  last_d  = (int)(prev % (seq_t)10);
        seq_t next_val = find_next(prev, last_d);
        cache[i] = next_val;
        computed[i] = 1;
    }
    last_computed = n;
    return cache[n];
}
