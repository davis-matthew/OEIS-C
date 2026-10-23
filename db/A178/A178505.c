/*
 * A178505  Decimal form of the period of 1/n for n such that gcd(n,10)=1.
 *          Leading zeros are suppressed.
 * https://oeis.org/A178505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A060284(A045572(n+1)) – period of 1/k where k is the *
 *            (n+1)-th integer coprime with 10. The period is obtained as the
 *            * integer (10^t – 1)/k where t is the multiplicative order of 10
 *            modulo k
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   multiplicative_order, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/multiplicative_order.h"

/* ------------------------------------------------------------------ */
/*  State:  cache for a(n) (1‑based index).  values[i] holds a(i),
 *          computed[i] indicates whether a(i) has been computed.  */
static seq_t *values   = NULL;
static bool  *computed = NULL;
static size_t cache_sz = 0;

/* ------------------------------------------------------------------ */
/*  State:  list of integers coprime with 10 (1‑based).  */
static unsigned int *coprime   = NULL;
static size_t coprime_used   = 0;
static size_t coprime_alloc  = 0;
static unsigned int next_candidate = 1;

/* ------------------------------------------------------------------ */
/*  Ensure that coprime[0..need-1] are filled.  */
static void
ensure_coprime(size_t need)
{
    if (coprime_used >= need)
        return;

    if (coprime_alloc < need) {
        size_t new_cap = coprime_alloc ? coprime_alloc * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        unsigned int *tmp = (unsigned int *)realloc(coprime,
                                                    new_cap * sizeof(unsigned int));
        /* If realloc fails, abort – the test harness expects non‑failing. */
        if (!tmp) exit(EXIT_FAILURE);
        /* Zero the newly allocated area. */
        memset(tmp + coprime_alloc, 0,
               (new_cap - coprime_alloc) * sizeof(unsigned int));
        coprime      = tmp;
        coprime_alloc = new_cap;
    }

    while (coprime_used < need) {
        /* Skip numbers divisible by 2 or 5. */
        while (!( (next_candidate & 1) && (next_candidate % 5) ))
            ++next_candidate;
        coprime[coprime_used++] = next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; keep 0 for safety */

    /* Expand cache if necessary. */
    while (cache_sz <= (size_t)n) {
        size_t new_cap = cache_sz ? cache_sz * 2 : 16;
        /* enlarge values array */
        seq_t *vtmp   = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!vtmp) exit(EXIT_FAILURE);
        /* enlarge computed flags array */
        bool *ctmp = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (!ctmp) exit(EXIT_FAILURE);

        /* Zero the newly allocated space */
        memset(ctmp + cache_sz, 0,
               (new_cap - cache_sz) * sizeof(bool));

        values   = vtmp;
        computed = ctmp;
        cache_sz = new_cap;
    }

    if (computed[(size_t)n])   /* already computed */
        return values[(size_t)n];

    /* Find k = (n+1)-th integer coprime with 10. */
    size_t k_index = (size_t)n + 1;
    ensure_coprime(k_index);
    unsigned int k = coprime[k_index - 1];

    /* Compute period length t. */
    unsigned long long t = multiplicative_order((unsigned long long)k);

    /* (10^t – 1) / k  */
    seq_t pow_t = pow10((idx_t)t);        /* 10^t */
    seq_t num   = pow_t - 1;              /* 10^t – 1 */
    seq_t res   = num / (seq_t)k;         /* decimal repeating block */

    values[(size_t)n]   = res;
    computed[(size_t)n] = true;
    return res;
}
