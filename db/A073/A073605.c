/*
 * A073605  Smallest number m such that m + k == 0 mod k-th prime for all k
 *          from 1 to n.
 * https://oeis.org/A073605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest non‑negative integer m such that * m + k ≡
 *            0 (mod p_k) for all k = 1..n, where p_k is the k‑th prime. *
 *            Computation: iteratively solve the Chinese Remainder Theorem
 *            using * modular inverses of the current product modulo each
 *            successive prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Prime generation ---------- */
#define MAX_SIEVE 200000          /* > 104729 (10000th prime) */
#define MAX_PRIMES 20000          /* more than primes ≤ MAX_SIEVE */

static uint8_t sieve_arr[MAX_SIEVE + 1];   /* 0 = prime candidate, 1 = composite */
static uint64_t prime_tbl[MAX_PRIMES];
static size_t prime_cnt = 0;

/* initialize prime table once */
static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already initialized */

    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (!sieve_arr[i]) {             /* i is prime */
            if (prime_cnt < MAX_PRIMES)
                prime_tbl[prime_cnt++] = (uint64_t)i;
            if ((long long)i * i <= MAX_SIEVE) {
                for (int j = i * i; j <= MAX_SIEVE; j += i)
                    sieve_arr[j] = 1;
            }
        }
    }
}

/* ---------- modular exponentiation (uint64_t) ---------- */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety, offset starts at 1 */

    init_primes();

    if (n > prime_cnt)                  /* shouldn't happen for n≤10000 */
        return 0;

    seq_t m = 0;                        /* current solution modulo P */
    seq_t P = 1;                        /* product of processed primes */

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t p = prime_tbl[(size_t)k - 1];
        uint64_t r = (p - (k % p)) % p;         /* required residue: m ≡ -k (mod p) */

        uint64_t m_mod = (uint64_t)(m % p);
        uint64_t diff  = (r + p - m_mod) % p;

        uint64_t P_mod = (uint64_t)(P % p);
        uint64_t inv    = modpow64(P_mod, p - 2, p);   /* inverse of P_mod modulo p */

        uint64_t t = (diff * inv) % p;

        m = m + P * (seq_t)t;                      /* update solution */
        P = P * (seq_t)p;                          /* update product of primes */
    }

    return m;
}
