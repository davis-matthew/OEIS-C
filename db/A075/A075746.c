/*
 * A075746  Numbers n such that 210*n-13 is prime.
 * https://oeis.org/A075746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth positive integer k such that 210*k-13 is prime
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- fast modular arithmetic for 64‑bit numbers ----------------------------------*/
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1)
            res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        exp >>= 1;
    }
    return res;
}

/*--- deterministic Miller‑Rabin for 64‑bit integers ------------------------------*/
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*--- main sequence routine ---------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(1) is the first term; for n==0 return 0 (undefined). */
    if (n == 0) return 0;

    static seq_t *sol = NULL;   /* array of found n‑values */
    static size_t sol_cap = 0;  /* allocated capacity */
    static size_t sol_cnt = 0;  /* number of solutions stored */
    static idx_t next = 1;      /* next integer to test */

    while (sol_cnt < n) {
        /* compute 210*next-13; use 128‑bit to avoid overflow for large n */
        uint64_t value = (uint64_t) ( (unsigned __int128)210 * next - 13 );

        if (is_prime_u64(value)) {
            /* grow the array if necessary */
            if (sol_cnt == sol_cap) {
                size_t new_cap = sol_cap ? sol_cap * 2 : 16;
                seq_t *new_sol = (seq_t *)realloc(sol, new_cap * sizeof(seq_t));
                if (!new_sol) /* allocation failed – abort */
                    return 0;
                sol = new_sol;
                sol_cap = new_cap;
            }
            sol[sol_cnt++] = (seq_t)next;   /* store the integer itself */
        }
        ++next;
    }
    return sol[n - 1];        /* a(1) is sol[0], a(2) is sol[1], ... */
}
