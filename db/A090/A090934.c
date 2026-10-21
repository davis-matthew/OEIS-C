/*
 * A090934  Primes such that least significant digit swapped with all other
 *          digits yields primes.
 * https://oeis.org/A090934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth prime whose decimal digits, when the least
 *            significant digit is swapped with every other digit
 *            individually, produce only primes as well
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Includes ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Forward declarations ---------- */
static inline bool is_prime_u64(uint64_t n);
static inline uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod);
static inline uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod);
static bool has_swap_property(uint64_t cand);

/* ---------- Prime test helpers ---------- */
static inline uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result = modmul(result, base, mod);
        base = modmul(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small prime trial division first */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Miller‑Rabin deterministic for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1ULL || x == n - 1ULL)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = modmul(x, x, n);
            if (x == n - 1ULL) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Property checker ---------- */
static bool has_swap_property(uint64_t cand)
{
    /* Extract digits (least significant first) */
    uint8_t digits[64];
    size_t len = 0;
    uint64_t tmp = cand;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10ULL);
        tmp /= 10ULL;
    }
    if (len == 0)
        len = 1; /* handle cand == 0, though never used */
    if (len == 1)
        return true; /* single digit primes are automatically valid */

    /* Pre‑compute powers of 10 */
    uint64_t pow10[64];
    pow10[0] = 1ULL;
    for (size_t i = 1; i < len; ++i)
        pow10[i] = pow10[i - 1] * 10ULL;

    uint8_t lsd = digits[0];
    for (size_t i = 1; i < len; ++i) {
        uint8_t di = digits[i];
        uint64_t swapped = cand;
        swapped -= (uint64_t)di * pow10[i];
        swapped += (uint64_t)lsd * pow10[i];
        swapped -= (uint64_t)lsd;
        swapped += (uint64_t)di;
        if (!is_prime_u64(swapped))
            return false;
    }
    return true;
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* off‑by‑one guard, not used */

    /* Static storage for discovered terms */
    static seq_t *terms = NULL;
    static size_t terms_sz = 0, terms_cap = 0;

    /* Static counter for next candidate prime (uint64_t keeps range safe) */
    static uint64_t current_candidate = 2;

    /* Ensure array capacity */
    if (terms_sz == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Generate terms until we have the requested index */
    while (terms_sz < (size_t)n) {
        uint64_t cand = current_candidate;

        /* Advance candidate: skip even numbers after 2 */
        if (cand > 2 && (cand & 1ULL) == 0ULL)
            cand += 1;

        /* primality test */
        if (is_prime_u64(cand) && has_swap_property(cand)) {
            terms[terms_sz++] = (seq_t)cand;
        }

        current_candidate = cand + 1;
    }

    return terms[n - 1];
}
