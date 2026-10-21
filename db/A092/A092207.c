/*
 * A092207  Semiprimes k such that k+2 is also a semiprime.
 * https://oeis.org/A092207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092207: semiprimes k such that k+2 is also a semiprime.
   The function computes a(n) for n ≥ 1.  It pre‑computes
   all terms up to 10000 and caches the result.  The code
   uses a simple Eratosthenes sieve and trial division
   to test semiprimality. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 2 000 000 is enough to contain the first 10 000 terms. */
#define MAX_LIMIT 2000000
#define MAX_TERMS 10000

/* static data used by the algorithm */
static bool is_composite[MAX_LIMIT + 1];
static uint32_t primes[150000];  /* π(2 000 000) ≈ 148 933 */
static size_t primes_cnt = 0;

static seq_t seq[MAX_TERMS];
static size_t seq_len = 0         /* number of terms already found */
    , seq_next_start = 4          /* next integer to test */
    , generated = 0;              /* 0 → run once, 1 → already done */

/* ------------------------------------------------------------ */
/* Build primes up to MAX_LIMIT using a simple sieve.           */
/* ------------------------------------------------------------ */
static void build_primes(void)
{
    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            /* Mark multiples of i.  i*i may overflow 32‑bit but
               fits in size_t (at least 64‑bit). */
            if ((size_t)i * i <= (size_t)MAX_LIMIT) {
                for (size_t j = i * i; j <= MAX_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Return true if x is a semiprime (product of two primes,
   possibly equal).                                              */
/* ------------------------------------------------------------ */
static bool is_semiprime(uint32_t x)
{
    if (x < 4) return false;
    uint32_t t = x;
    int cnt = 0;
    for (size_t k = 0; k < primes_cnt && (uint64_t)primes[k]*primes[k] <= (uint64_t)t; ++k) {
        uint32_t p = primes[k];
        while (t % p == 0) {
            ++cnt;
            t /= p;
            if (cnt > 2) return false;
        }
    }
    if (t > 1) ++cnt;            /* remaining prime factor */
    return cnt == 2;
}

/* ------------------------------------------------------------ */
/* Generate all sequence terms up to MAX_TERMS.                  */
/* ------------------------------------------------------------ */
static void generate_all(void)
{
    if (generated) return;
    build_primes();

    seq_len = 0;
    seq_next_start = 4;
    while (seq_len < MAX_TERMS) {
        uint32_t i = (uint32_t)seq_next_start;
        /* Advance to the first candidate that might be a twin semiprime. */
        while (i <= MAX_LIMIT - 2) {
            if (is_semiprime(i) && is_semiprime(i + 2)) {
                seq[seq_len++] = (seq_t)i;
                if (seq_len == MAX_TERMS) break;
            }
            ++i;
        }
        seq_next_start = (size_t)i;
    }
    generated = 1;
}

/* ------------------------------------------------------------ */
/* Public sequence function.                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined behaviour for n==0, but we guard */
        return (seq_t)0;
    generate_all();
    return seq[(size_t)n - 1];
}
