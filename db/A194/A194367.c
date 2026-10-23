/*
 * A194367  Smallest k such that prime(n) divides k*prime(n+1)+1.
 * https://oeis.org/A194367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Smallest k>0 such that prime(n) divides k*prime(n+1)+1
 * Range:     u8 n <= 61, u16 n <= 6801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime storage and generation ---------- */
static uint64_t *prime_list = NULL;   /* 1‑based index: prime_list[0] = 2 */
static size_t     prime_count = 0;    /* number of primes stored */
static size_t     prime_cap   = 0;    /* current allocation size */
static uint64_t   next_candidate = 2; /* next number to test for primality */

/* enlarge prime_list array when needed ---------------------------------- */
static void grow_prime_list(void) {
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) { free(prime_list); exit(EXIT_FAILURE); }
    prime_list = tmp;
    prime_cap = new_cap;
}

/* trial‑division test against already known primes --------------------- */
static bool is_prime_trial(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ensure that at least 'need' primes are available (prime_count >= need)  */
static void ensure_primes(idx_t need) {
    if (prime_count >= (size_t)need) return;
    while (prime_count < (size_t)need) {
        if (next_candidate < 2) next_candidate = 2;
        if (is_prime_trial(next_candidate)) {
            if (prime_count == prime_cap) grow_prime_list();
            prime_list[prime_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- Extended Euclidean algorithm for modular inverse ---------- */
static int64_t extgcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    if (b == 0) { *x = 1; *y = 0; return a; }
    int64_t x1, y1;
    int64_t d = extgcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return d;
}

static uint64_t mod_inv(uint64_t a, uint64_t m) {
    /* a and m are < 2^64, m is prime (< a may not hold) */
    int64_t x, y;
    extgcd((int64_t)a, (int64_t)m, &x, &y);
    int64_t inv = x % (int64_t)m;
    if (inv < 0) inv += m;
    return (uint64_t)inv;
}

/* ---------- Sequence function ----------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;       /* undefined for n=0 (offset is 1) */

    /* Need prime(n) and prime(n+1) */
    ensure_primes(n + 1);

    uint64_t p_n   = prime_list[n - 1];
    uint64_t p_np1 = prime_list[n];

    /* Compute inverse of prime(n+1) modulo prime(n) */
    uint64_t mod = p_n;
    uint64_t a   = p_np1 % mod;
    uint64_t inv = mod_inv(a, mod);          /* 0 <= inv < mod */

    uint64_t k = mod - inv;                  /* k ≡ -inv (mod mod) */
    if (k == 0) k = mod;                     /* in case inv == 0, keep k>0 */

    return (seq_t)k;
}
