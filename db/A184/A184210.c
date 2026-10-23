/*
 * A184210  Number of (n+1)X3 0..3 arrays with every 2X2 subblock sum greater
 *          than 6.
 * https://oeis.org/A184210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            21*a(n-1)+243*a(n-2)-1800*a(n-3)-8335*a(n-4)+43037*a(n-5)+82290*a(n-6)-395548*a(n-7)-259095*a(n-8)+1530499*a(n-9)+86017*a(n-10)-2608822*a(n-11)+581376*a(n-12)+2049624*a(n-13)-748352*a(n-14)-707040*a(n-15)+310464*a(n-16)+86400*a(n-17)-41472*a(n-18)
 *            (for n>18). Initial values: a(1)=1036, a(2...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The following external types are supplied by the build system */
extern seq_t a(idx_t n);

/* Internal data structures */
#define STATE_BITS 6
#define STATES 64

static seq_t T[STATES][STATES];
static bool T_inited = false;

/* Transition matrix initialisation */
static void init_T(void) {
    if (T_inited) return;
    /* decode each pattern into 3 digits base 4 */
    for (idx_t s = 0; s < STATES; ++s) {
        uint8_t a0 = s & 3;
        uint8_t a1 = (s >> 2) & 3;
        uint8_t a2 = (s >> 4) & 3;
        for (idx_t t = 0; t < STATES; ++t) {
            uint8_t b0 = t & 3;
            uint8_t b1 = (t >> 2) & 3;
            uint8_t b2 = (t >> 4) & 3;
            bool ok = true;
            /* first 2x2 block (columns 0-1) */
            if (a0 + a1 + b0 + b1 <= 6) ok = false;
            /* second 2x2 block (columns 1-2) */
            if (a1 + a2 + b1 + b2 <= 6) ok = false;
            T[s][t] = ok ? (seq_t)1 : (seq_t)0;
        }
    }
    T_inited = true;
}

/* Cache for pre‑computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure capacity for at least min_cap entries */
static void ensure_cache(idx_t min_cap) {
    if (min_cap <= cache_cap) return;
    size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 64;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1); /* allocation failure */
    cache = new_buf;
    cache_cap = (idx_t)new_cap;
}

/* Pre‑compute the first 18 terms using a direct DP over rows */
static void compute_base_terms(void) {
    if (cache_len >= 18) return;
    ensure_cache(18);
    seq_t dp[STATES], dp_next[STATES];
    for (idx_t i = 0; i < STATES; ++i) dp[i] = 1;   /* first row */
    for (idx_t step = 1; step <= 18; ++step) {
        for (idx_t i = 0; i < STATES; ++i) dp_next[i] = 0;
        for (idx_t s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            for (idx_t t = 0; t < STATES; ++t)
                if (T[s][t]) dp_next[t] += val;
        }
        seq_t total = 0;
        for (idx_t i = 0; i < STATES; ++i) total += dp_next[i];
        cache[step - 1] = total;
        /* prepare for next iteration */
        for (idx_t i = 0; i < STATES; ++i) dp[i] = dp_next[i];
    }
    cache_len = 18;
}

/* The recurrence coefficients (18 of them) */
static const int coeffs[18] = {
    21, 243, -1800, -8335, 43037, 82290, -395548, -259095,
    1530499, 86017, -2608822, 581376, 2049624, -748352, -707040,
    310464, 86400, -41472
};

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset 1; undefined case */
    init_T();
    compute_base_terms();
    if (n <= cache_len) return cache[n - 1];

    /* Compute additional terms up to n using the recurrence */
    while (cache_len < n) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 1; i <= 18; ++i) {
            seq_t val = cache[cache_len - i];
            int c = coeffs[i - 1];
            if (c > 0) pos += val * (seq_t)c;
            else       neg += val * (seq_t)(-c);
        }
        seq_t next = pos - neg;
        ensure_cache(cache_len + 1);
        cache[cache_len] = next;
        ++cache_len;
    }
    return cache[n - 1];
}
