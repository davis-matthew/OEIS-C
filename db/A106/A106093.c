/*
 * A106093  Primes with maximal digit = 9.
 * https://oeis.org/A106093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2945, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_nine, mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106093: primes whose maximal decimal digit is 9 (i.e., primes containing at least one digit 9).
   a(n) = the n-th such prime, with n ≥ 1.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>   /* for size_t */
#include "../utils/has_nine.h"
#include "../utils/mulmod64.h"

/* ----------------------------------------------------------------------
   Primality test --------------------------------------------------------
   ---------------------------------------------------------------------- */
/* Deterministic Miller–Rabin for 64‑bit integers.
   Witnesses  {2, 3, 5, 7, 11, 13} are enough for n < 3·10^18. */

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mulmod64(result, b, mod);
        b = mulmod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;
    if (n % 7 == 0) return n == 7;
    /* trial division by small primes up to 100
       to speed up the Miller–Rabin step */
    static const uint16_t small_primes[] = {
        11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    const uint64_t witnesses[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue; /* skip if witness divisible by n */
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned int r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------
   Data storage ----------------------------------------------------------
   ---------------------------------------------------------------------- */
/* Capacity of the static array.  The 10 000‑th prime with a ‘9’
   is below 200 000, so this size is safely oversized. */
#define PRIME_CAP 200000

static seq_t prime_seq[PRIME_CAP];
static idx_t prime_cnt = 0;      /* number of primes stored so far */
static uint64_t last_checked = 1;/* last integer examined */

/* ----------------------------------------------------------------------
   Generation ------------------------------------------------------------
   ---------------------------------------------------------------------- */
static void generate_up_to(idx_t target) {
    while (prime_cnt < target) {
        ++last_checked;

        /* Skip even numbers after 2 */
        if (last_checked > 2 && (last_checked & 1) == 0) continue;

        if (is_prime_uint64(last_checked) && has_nine(last_checked)) {
            prime_seq[prime_cnt++] = (seq_t)last_checked;
            /* In the unlikely event the array overflows, abort; the
               sequence is only required for n ≤ ~10 000. */
            if (prime_cnt == PRIME_CAP) return;
        }
    }
}

/* ----------------------------------------------------------------------
   Public API ------------------------------------------------------------
   ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined region – safe default */
    if (prime_cnt < n) generate_up_to(n);
    return prime_seq[n - 1];
}
