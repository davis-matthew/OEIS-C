/*
 * A083173  Triangle read by rows: the n-th row contains the first n-1
 *          multiples of prime(n) followed by the next multiple that will make
 *          the row sum a multiple of n.
 * https://oeis.org/A083173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: A083173 – triangle read by rows.
   For a given row index r (row r has r entries)
   the first r-1 entries are p*r where p is the r‑th prime
   and the r‑th entry is p*k, where k is the smallest integer
   ≥ r such that the sum of the row is divisible by r. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime table (dynamic) ---------- */
static uint64_t *prime_cache = NULL;   /* array of primes          */
static size_t    prime_cache_len = 0;   /* number of primes stored  */
static size_t    prime_cache_cap = 0;   /* capacity of the array    */

/* ensure at least 'need' primes are stored. */
static void ensure_primes(idx_t need)
{
    if (prime_cache_len >= need)
        return;                          /* already have enough       */

    /* grow the array as needed */
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                        new_cap * sizeof(uint64_t));
    if (!tmp) {
        /* allocation failed – abort – this should never happen in tests */
        exit(EXIT_FAILURE);
    }
    prime_cache = tmp;
    prime_cache_cap = new_cap;

    /* generate primes starting after last known prime */
    uint64_t candidate = prime_cache_len ? prime_cache[prime_cache_len - 1] + 1 : 2;
    while (prime_cache_len < (size_t)need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cache_len; ++i) {
            uint64_t p = prime_cache[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_cache_len++] = candidate;
        }
        ++candidate;
    }
}

/* modular inverse of a modulo mod (gcd(a,mod)=1, mod>1) */
static uint64_t modinv(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = t - (int64_t)q * newt;
        t = newt;  newt = tmp;
        tmp = r - (int64_t)q * newr;
        r = newr;  newr = tmp;
    }
    if (r > 1) return 0;         /* no inverse – should not happen */
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 – return 0 */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n)
        ++r;

    /* position inside the row (1-based) */
    idx_t pos = n - ( (idx_t)(((__int128)(r - 1) * r) / 2) ); /* pos in [1..r] */

    /* fetch r‑th prime */
    ensure_primes(r);
    uint64_t p = prime_cache[r - 1];
    seq_t sp = (seq_t)p;

    /* row 1 has only the prime itself */
    if (r == 1)
        return sp;

    if (pos < r) {
        /* first r-1 entries: simple multiples */
        return sp * (seq_t)pos;
    }

    /* pos == r: compute the special multiple k >= r */
    /* S = p * ((r-1)*r/2)   (sum of first r-1 multiples) */
    /* S_mod = (p % r) * (((r-1)*r/2) % r) % r */
    uint64_t M_mod = (uint64_t)(( (__int128)(r - 1) * r / 2 ) % r);   /* (r-1)*r/2 modulo r */
    uint64_t S_mod = ( (p % r) * M_mod ) % r;                       /* modulo r */

    /* compute inverse of p modulo r */
    uint64_t inv = modinv(p % r, r);  /* p and r are coprime (p > r) */

    /* k modulo r */
    uint64_t k_mod = (( (r - S_mod) % r ) * inv) % r;

    /* minimal k >= r */
    uint64_t k = k_mod;
    if (k < (uint64_t)r)
        k += (uint64_t)r;

    return sp * (seq_t)k;
}
