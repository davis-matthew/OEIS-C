/*
 * A040178  Primes p such that x^5 = 10 has no solution mod p.
 * https://oeis.org/A040178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 974, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, powmod_big
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
#include "../utils/is_prime_big.h"
#include "../utils/powmod_big.h"

/* --- dynamic storage of unsolvable primes --- */
static seq_t *unsol_vec = NULL;  /* array */
static size_t unsol_len = 0;     /* number of elements in use */
static size_t unsol_cap = 0;     /* allocated capacity */

/* append a new unsolvable prime */
static void append_unsol(seq_t val)
{
    if (unsol_len == unsol_cap) {
        size_t new_cap = unsol_cap ? unsol_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(unsol_vec, new_cap * sizeof(seq_t));
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
