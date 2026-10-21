/*
 * A057263  Numbers n such that n | 12^n + 11^n + 10^n + 9^n + 8^n + 7^n +
 *          6^n.
 * https://oeis.org/A057263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057263: numbers n such that n divides 12ⁿ+11ⁿ+10ⁿ+9ⁿ+8ⁿ+7ⁿ+6ⁿ */
/* The function computes terms on demand by brute‑force testing candidates. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;          /* cache of found terms          */
static size_t terms_len = 0;         /* number of cached terms        */
static size_t terms_cap = 0;         /* allocated capacity            */

static void
ensure_terms(size_t needed)
{
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap << 1U : 8;
        while (new_cap < needed) new_cap <<= 1U;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

static void
push_term(seq_t val)
{
    ensure_terms(terms_len + 1);
    terms[terms_len++] = val;
}

/* modular exponentiation using 128‑bit intermediate to avoid overflow */
static inline uint64_t
mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1U;
    }
    return result;
}

/* Test candidate k; if it satisfies the divisibility condition, cache it. */
static void
check_candidate(idx_t k)
{
    if (k == 1) {               /* 1 always satisfies the condition */
        push_term((seq_t)1);
        return;
    }

    uint64_t mod = (uint64_t)k;
    uint64_t sum = 0;
    static const uint64_t bases[7] = {6,7,8,9,10,11,12};

    for (size_t i = 0; i < 7; ++i) {
        sum += mod_pow64(bases[i], k, mod);
        if (sum >= mod) sum -= mod;   /* keep sum mod mod to avoid overflow */
    }

    if (sum % mod == 0)
        push_term((seq_t)k);
}

/* Ensure that at least n terms have been generated and cached. */
static void
generate_up_to(idx_t n)
{
    if (terms_len == 0) {                  /* start with the trivial term 1 */
        push_term((seq_t)1);
        if (n == 1) return;
    }

    /* Continue testing all candidates from the next integer after the last checked. */
    uint64_t cand = terms_len == 1 ? 2 : (uint64_t)terms[terms_len - 1] + 1ULL;

    while (terms_len < n) {
        check_candidate(cand);
        ++cand;
    }
}

/* Required public interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, but safe guard */
    generate_up_to(n);
    return terms[n - 1];
}
