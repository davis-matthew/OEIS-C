/*
 * A045596  Numbers k that divide 7^k + 5^k.
 * https://oeis.org/A045596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k that divides 7^k + 5^k. The
 *            implementation searches k = 1, 2, …, testing each with modular
 *            exponentiation. Results are cached so that subsequent calls for
 *            any n are answered in O(1) after the initial search
 * Range:     u8 n <= 10, u16 n <= 39, u32 n <= 141, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline uint64_t pow_mod_u64(uint64_t base, idx_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

static void ensure_capacity_seq_t(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 4 : *cap << 1;
        while (new_cap < needed)
            new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset */

    /* Static state shared between calls */
    static seq_t *terms = NULL;      /* array of found k's */
    static size_t terms_cap = 0;     /* capacity of the array */
    static size_t terms_len = 0;     /* number of terms stored */
    static idx_t last_k = 0;         /* last k examined */

    while (terms_len < n) {
        ++last_k;

        /* Compute 7^last_k mod last_k and 5^last_k mod last_k */
        uint64_t mod = (uint64_t)last_k;
        uint64_t a_mod = pow_mod_u64(7, last_k, mod);
        uint64_t b_mod = pow_mod_u64(5, last_k, mod);
        uint64_t sum_mod = (a_mod + b_mod) % mod;

        if (sum_mod == 0) {
            ensure_capacity_seq_t(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)last_k;
        }
    }

    return terms[n - 1];
}
