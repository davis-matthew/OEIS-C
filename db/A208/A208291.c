/*
 * A208291  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical recurrence a(n) = 4*a(n-1) + 16*a(n-2) - 7*a(n-3) -
 *            23*a(n-4) + 2*a(n-5) + 9*a(n-6) - a(n-8) with initial values
 *            a(1)=14, a(2)=196, a(3)=966, a(4)=6650, a(5)=40376, a(6)=256774,
 *            a(7)=1604862. This implementation uses a DP over column patterns
 *            instead of the explicit recurrence
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- State ----- */
static bool initialized = false;
static size_t S = 0;                 /* number of allowed column patterns */
static uint8_t *allowed = NULL;      /* allowed patterns (indices 0..S-1) */
static uint16_t *allow_mask = NULL;  /* for each pair (p,q) bitmask of r that work */
static seq_t *dp_cur = NULL;         /* DP for current length (last two columns) */
static seq_t *dp_next = NULL;        /* DP for next length */
static seq_t *a_cache = NULL;        /* cached a(n) values */
static size_t cache_size = 0;        /* allocated size of a_cache */
static size_t max_n_seen = 0;        /* largest n for which a_cache is filled */

/* ---------- utilities ----- */
static void init(void)
{
    if (initialized)
        return;

    /* Find all 7‑bit column patterns that obey the vertical constraint
       (no 001 or 110 in any three consecutive rows). */
    uint8_t temp[128];
    size_t cnt = 0;
    for (int pat = 0; pat < 128; ++pat) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
            int b0 = (pat >> i) & 1;
            int b1 = (pat >> (i + 1)) & 1;
            int b2 = (pat >> (i + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 1 && b2 == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            temp[cnt++] = (uint8_t)pat;
        }
    }
    S = cnt;
    allowed = (uint8_t *)malloc(S * sizeof(uint8_t));
    for (size_t i = 0; i < S; ++i)
        allowed[i] = temp[i];

    /* Precompute transition masks: for each pair (p,q) store bitmask of r
       for which the horizontal triple (p,q,r) is legal at every row. */
    allow_mask = (uint16_t *)malloc(S * S * sizeof(uint16_t));
    for (size_t p = 0; p < S; ++p) {
        for (size_t q = 0; q < S; ++q) {
            uint16_t mask = 0;
            uint8_t patp = allowed[p];
            uint8_t patq = allowed[q];
            for (size_t r = 0; r < S; ++r) {
                uint8_t patr = allowed[r];
                bool ok = true;
                for (int row = 0; row < 7 && ok; ++row) {
                    int b1 = (patp >> row) & 1;
                    int b2 = (patq >> row) & 1;
                    int b3 = (patr >> row) & 1;
                    int triple = (b1 << 2) | (b2 << 1) | b3;
                    /* forbidden horizontal patterns: 000 and 001 */
                    if (triple == 0 || triple == 1)
                        ok = false;
                }
                if (ok)
                    mask |= (1U << r);
            }
            allow_mask[p * S + q] = mask;
        }
    }

    /* Allocate DP arrays for length 2 (last two columns). */
    size_t dp_sz = S * S;
    dp_cur = (seq_t *)malloc(dp_sz * sizeof(seq_t));
    dp_next = (seq_t *)malloc(dp_sz * sizeof(seq_t));

    /* Initialize cache for a(1) and a(2). */
    cache_size = 3;              /* we want indices 0..2, ignore 0 */
    a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!a_cache) abort();
    a_cache[0] = 0; /* unused */

    seq_t S_seq = (seq_t)S;
    a_cache[1] = S_seq;
    a_cache[2] = S_seq * S_seq;

    /* dp_cur for length 2: each pair counts as 1 */
    for (size_t idx = 0; idx < dp_sz; ++idx)
        dp_cur[idx] = 1;

    max_n_seen = 2;
    initialized = true;
}

/* ---------- main function ----- */
seq_t a(idx_t n)
{
    init();

    if (n <= max_n_seen)
        return a_cache[n];

    size_t dp_sz = S * S;

    while (max_n_seen < n) {
        /* compute next DP */
        for (size_t i = 0; i < dp_sz; ++i)
            dp_next[i] = 0;

        for (size_t p = 0; p < S; ++p) {
            for (size_t q = 0; q < S; ++q) {
                seq_t val = dp_cur[p * S + q];
                if (val == 0)
                    continue;
                uint16_t mask = allow_mask[p * S + q];
                /* iterate set bits in mask */
                for (uint16_t m = mask; m; m &= m - 1) {
                    uint16_t r = __builtin_ctz(m);
                    dp_next[q * S + r] += val;
                }
            }
        }

        /* Sum to get a_{max_n_seen+1} */
        seq_t a_next = 0;
        for (size_t i = 0; i < dp_sz; ++i)
            a_next += dp_next[i];

        /* Ensure cache is big enough */
        if ((size_t)n + 1 > cache_size) {
            size_t new_size = cache_size * 2;
            if ((size_t)n + 1 > new_size)
                new_size = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
            if (!tmp)
                abort();
            a_cache = tmp;
            cache_size = new_size;
        }
        a_cache[++max_n_seen] = a_next;

        /* Swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return a_cache[n];
}
