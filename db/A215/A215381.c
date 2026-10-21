/*
 * A215381  Primes congruent to {0, 1, 2, 3} mod 19.
 * https://oeis.org/A215381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 0, 1, 2 or 3 (mod 19)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array helpers ----------------------------------------------- */
static void grow_uint64_array(uint64_t **arr, size_t *cap) {
    if (*cap == 0) {
        *cap = 16;
        *arr = (uint64_t*)malloc(*cap * sizeof(uint64_t));
    } else {
        *cap <<= 1;
        *arr = (uint64_t*)realloc(*arr, *cap * sizeof(uint64_t));
    }
}

/* Prime testing (trial division) ---------------------------------------- */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

/* Global state ---------------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0, prime_cap  = 0;   /* not actively used */

static uint64_t *valid_list = NULL;
static size_t   valid_cnt  = 0, valid_cap  = 0;

static uint64_t candidate = 2;

/* Append a prime to the valid list ------------------------------------- */
static void maybe_append_valid(uint64_t p) {
    uint64_t r = p % 19;
    if (r <= 3) {          /* residues 0,1,2,3 */
        if (valid_cnt == valid_cap) grow_uint64_array(&valid_list, &valid_cap);
        valid_list[valid_cnt++] = p;
    }
}

/* Ensure the state is initialised with the first prime 2 ---------------- */
static void init_state(void) {
    if (prime_cnt == 0) {
        if (prime_cnt == 0) grow_uint64_array(&prime_list, &prime_cap);
        prime_list[prime_cnt++] = 2;
        maybe_append_valid(2);
        candidate = 3;
    }
}

/* Generate valid primes until we have at least n of them ---------------- */
static void ensure_valid_primes(idx_t n) {
    init_state();
    while (valid_cnt < n) {
        /* Find next prime */
        while (true) {
            if (candidate < 2) candidate = 2;
            if (is_prime_uint64(candidate)) {
                /* Store prime (optional) -------------------------------- */
                if (prime_cnt == prime_cap) grow_uint64_array(&prime_list, &prime_cap);
                prime_list[prime_cnt++] = candidate;
                /* Store if it meets the residue condition */
                maybe_append_valid(candidate);
                candidate++;                      /* next candidate */
                break;
            }
            candidate++;
        }
    }
}

/* Public function ------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1; 0 is undefined */
    ensure_valid_primes(n);
    return (seq_t)valid_list[n - 1];
}
