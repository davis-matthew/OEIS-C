/*
 * A191081  Primes that are not squares mod 79.
 * https://oeis.org/A191081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191081: primes that are not squares mod 79 */

/* The function returns the nth prime p (with n ≥ 1) such that the Legendre
   symbol (p|79) = −1.  The implementation builds the sequence on demand
   and memoises it, using a simple trial‑division primality test against the
   primes discovered so far.  The quadratic‑residue table for 79 is
   initialised only once.  This algorithm easily produces the first
   ten thousand terms in well under two seconds on typical hardware.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state ------------------------------------------------------- */
static seq_t *cached_seq = NULL;   /* cached sequence terms      */
static size_t cached_cap = 0;
static size_t cached_len = 0;

static uint64_t *prime_list = NULL;    /* primes used for trial division */
static size_t prime_cap  = 0;
static size_t prime_len  = 0;

static uint64_t current_candidate = 2;  /* next integer to test for primality */

/* table of quadratic residues modulo 79 */
static uint8_t isQR[79];
static bool qr_done = false;

/* helper: grow the prime list (no overflow check needed for small inputs) */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* helper: grow the cached sequence */
static void add_value(seq_t val)
{
    if (cached_len == cached_cap) {
        size_t new_cap = cached_cap ? cached_cap << 1 : 8;
        seq_t *tmp = (seq_t*)realloc(cached_seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cached_seq = tmp;
        cached_cap = new_cap;
    }
    cached_seq[cached_len++] = val;
}

/* initialise the quadratic‑residue table for 79 */
static void init_qr(void)
{
    if (qr_done) return;
    for (int i = 0; i < 79; ++i) isQR[i] = 0;
    for (int i = 0; i < 79; ++i) {
        int r = (i * i) % 79;
        isQR[r] = 1;
    }
    qr_done = true;
}

/* main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1 but avoid UB */

    /* ensure residue table ready */
    init_qr();

    /* extend the cache until we have at least n terms */
    while (cached_len < (size_t)n) {
        uint64_t cand = current_candidate;

        /* primality test using previously found primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;   /* avoids overflow in p*p */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store the new prime for future checks */
            add_prime(cand);

            /* test the quadratic‑residue condition (skip 79 itself) */
            if (cand != 79) {
                uint64_t r = cand % 79;
                if (!isQR[r]) add_value((seq_t)cand);
            }
        }

        /* advance to the next candidate: 2 → 3 → 5 → 7 → ... */
        if (cand == 2)
            current_candidate = 3;
        else
            current_candidate += 2;   /* only odd numbers after 2 */

        /* break early if the required term has been found */
        if (cached_len >= (size_t)n) break;
    }

    return cached_seq[(size_t)n - 1];
}
