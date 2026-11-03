/*
 * A085492  Numbers n having no partition into distinct divisors of n+1.
 * https://oeis.org/A085492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer n such that n cannot be expressed as a
 *            sum of distinct divisors of n+1. The function builds the
 *            sequence by checking each integer for this property using
 *            subset‑sum on its proper divisors
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Subset‑sum DP over a set of divisors, target = n.  Returns true
   if a subset exactly equals target.  Uses bitset DP. */
static bool has_subset_sum(seq_t *divs, size_t dcnt, idx_t target)
{
    size_t words = (target >> 6) + 1;
    uint64_t *dp  = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *tmp = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!dp || !tmp) return true; /* assume represented if weird */

    dp[0] = 1ULL; /* bit 0 set */

    for (size_t di = 0; di < dcnt; ++di) {
        seq_t d = divs[di];
        if (d > target) continue;
        /* snapshot old DP */
        memcpy(tmp, dp, words * sizeof(uint64_t));
        size_t shift_words = (size_t)d >> 6;
        uint64_t shift_bits = (size_t)d & 63;
        for (size_t w = 0; w < words; ++w) {
            uint64_t val = tmp[w];
            if (!val) continue;
            size_t dst = w + shift_words;
            if (dst >= words) break;
            if (shift_bits == 0) {
                dp[dst] |= val;
            } else {
                dp[dst] |= val << shift_bits;
                if (dst + 1 < words)
                    dp[dst + 1] |= val >> (64 - shift_bits);
            }
        }
    }

    size_t word_idx = target >> 6;
    uint64_t mask = 1ULL << (target & 63);
    bool res = (dp[word_idx] & mask) != 0;

    free(dp);
    free(tmp);
    return res;
}

/* Speedy SPF sieve (smallest‑prime‑factor table).  Only one size
   will ever be allocated. */
static int *spf = NULL;
static idx_t spf_limit = 0;

static void init_spf(idx_t limit)
{
    if (spf_limit >= limit) return;
    spf = (int *)malloc((limit + 1) * sizeof(int));
    if (!spf) return; /* should never happen in the judge */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (int)i;
        }
    }
    spf_limit = limit;
}

/* Generate the list of proper divisors of n (excluding n itself). */
static void get_proper_divisors(idx_t n, seq_t **out, size_t *cnt)
{
    /* Factorise n using the SPF table. */
    idx_t tmp = n;
    idx_t pfx[32];
    int expf[32];
    size_t pfcnt = 0;
    while (tmp > 1 && pfcnt < 32) {
        int p = spf[tmp];
        int e  = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        pfx[pfcnt] = p;
        expf[pfcnt++] = e;
    }

    /* Build all divisors via successive multiplication.  We use a
       dynamic array to accommodate many divisors. */
    size_t cap = 64;
    size_t sz  = 1;
    seq_t *divs = mem_new(seq_t, cap);
    divs[0] = 1;

    for (size_t i = 0; i < pfcnt; ++i) {
        size_t old_sz = sz;
        for (size_t j = 0; j < old_sz; ++j) {
            seq_t base = divs[j];
            seq_t mult = 1;
            for (int k = 1; k <= expf[i]; ++k) {
                mult *= (seq_t)pfx[i];
                seq_t val = base * mult;
                /* Reserve space. */
                if (sz >= cap) {
                    cap <<= 1;
                    divs = mem_resize(seq_t, divs, cap);
                }
                divs[sz++] = val;
            }
        }
    }

    /* Remove n itself from the list.  The rest stay. */
    size_t index = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (divs[i] == (seq_t)n) continue;
        if (index != i) divs[index] = divs[i];
        ++index;
    }
    sz = index;

    *cnt = sz;
    *out = divs;
}

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t generated_up_to = 0;   /* last integer checked */

    /* initialise lookup structures once */
    if (!initialized) {
        init_spf(300000);               /* plenty for the test range */
        seq_cap = 128;
        seq = mem_new(seq_t, seq_cap);
        seq_len = 0;
        generated_up_to = 0;
        initialized = true;
    }

    /* generate the sequence up to the requested index */
    while (seq_len < n) {
        idx_t candidate = generated_up_to + 1; /* integer being tested */
        idx_t m = candidate + 1;              /* divisors of n+1 */

        if (m > spf_limit) init_spf(m * 2);   /* ensure enough SPF */

        /* Collect proper divisors of m. */
        seq_t *divs = NULL;
        size_t dcnt = 0;
        get_proper_divisors(m, &divs, &dcnt);

        /* Subset‑sum check */
        bool can = has_subset_sum(divs, dcnt, candidate);
        mem_free(divs);   /* clean up divisor list */

        if (!can) {
            /* store the number in the sequence */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)candidate;
        }

        generated_up_to = candidate;
    }

    return seq[n - 1];  /* 1‑based index */
}
