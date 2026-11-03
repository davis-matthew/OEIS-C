/*
 * A040178  Primes p such that x^5 = 10 has no solution mod p.
 * https://oeis.org/A040178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p such that the congruence
   x^5 ≡ 10 (mod p) has no solution (OEIS A040178).  
   For primes p > 5 the condition is:
      if p % 5 != 1   → always solvable;
      if p % 5 == 1   → solvable iff 10^((p‑1)/5) ≡ 1 (mod p).
   The function builds the sequence on demand, storing found primes
   in a static array for reuse. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- auxiliary: modular exponentiation for big exponents --- */
static seq_t powmod_big(seq_t base, seq_t exp, seq_t mod)
{
    if (mod == (seq_t)0) return (seq_t)0;
    seq_t result = (seq_t)1;
    base = base % mod;
    while (exp > (seq_t)0) {
        if (exp % (seq_t)2 == (seq_t)1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / (seq_t)2;
    }
    return result;
}

/* --- dynamic storage of unsolvable primes --- */
static seq_t *unsol_vec = NULL;  /* array */
static size_t unsol_len = 0;     /* number of elements in use */
static size_t unsol_cap = 0;     /* allocated capacity */

/* append a new unsolvable prime */
static void append_unsol(seq_t val)
{
    if (unsol_len == unsol_cap) {
        size_t new_cap = unsol_cap ? unsol_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, unsol_vec, new_cap);
        unsol_vec = tmp;
        unsol_cap = new_cap;
    }
    unsol_vec[unsol_len++] = val;
}

/* ensure we have at least n unsolvable primes stored */
static void ensure_unsol(idx_t n)
{
    /* start searching from the first odd prime greater than 5 */
    static seq_t current = (seq_t)7;
    while (unsol_len < n) {
        if (is_prime_big(current)) {
            seq_t mod5 = current % (seq_t)5;
            if (mod5 == (seq_t)1) {
                seq_t exp = (current - (seq_t)1) / (seq_t)5;
                seq_t res = powmod_big((seq_t)10, exp, current);
                if (res != (seq_t)1) {
                    append_unsol(current);
                }
            }
        }
        current += (seq_t)2;   /* skip even numbers */
    }
}

/* the required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1 – undefined for n=0 */
    ensure_unsol(n);
    return unsol_vec[n - 1];
}
