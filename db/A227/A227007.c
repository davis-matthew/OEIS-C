/*
 * A227007  Numbers k such that k-1 is squarefree and every prime divisor of
 *          k-1 is in the sequence.
 * https://oeis.org/A227007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = k such that k‑1 is squarefree and every prime divisor of
 *            k‑1 is an element of the sequence itself
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* static storage for the sequence and helper tables */
static seq_t seq_array[2000];
static size_t seq_len = 0;
static bool in_seq[2001];
static int prime_list[2000];
static size_t prime_cnt = 0;

/* ------------------------------------------------------------------ */
/* compute the sequence up to the largest known term (1807).          */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    /* ------------------------------------------------------------ */
    /* generate all primes up to 2000 (sufficient for our limits)   */
    /* ------------------------------------------------------------ */
    const idx_t MAX_PRIME = 2000;
    bool is_prime[MAX_PRIME + 1];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = false;
    is_prime[1] = false;
    for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i) {
                is_prime[j] = false;
            }
        }
    }
    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            prime_list[prime_cnt++] = (int)i;
        }
    }

    /* ------------------------------------------------------------ */
    /* evaluate candidate k in increasing order                     */
    /* ------------------------------------------------------------ */
    const idx_t MAX_K = 2000;          /* exceeds the known maximum 1807 */
    memset(in_seq, 0, sizeof(in_seq));

    for (idx_t k = 2; k <= MAX_K; ++k) {
        uint64_t m = k - 1;
        uint64_t temp = m;
        bool ok = true;

        /* factor m, test for squarefree and membership of prime divisors */
        for (size_t pi = 0; pi < prime_cnt && (uint64_t)prime_list[pi] * prime_list[pi] <= temp; ++pi) {
            uint64_t p = prime_list[pi];
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    ++cnt;
                    temp /= p;
                    if (cnt > 1) {          /* not squarefree */
                        ok = false;
                        break;
                    }
                }
                if (!ok) break;
                if (!in_seq[p]) {           /* prime divisor not yet in sequence */
                    ok = false;
                    break;
                }
            }
        }
        if (!ok) continue;

        if (temp > 1) {                      /* remaining prime factor */
            if (!in_seq[temp]) continue;    /* not in the sequence */
        }

        /* k satisfies all conditions: store it */
        seq_array[seq_len++] = (seq_t)k;
        in_seq[k] = true;
    }
}

/* ------------------------------------------------------------------ */
/* public entry point                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (seq_len == 0) init_sequence();   /* lazily compute once          */

    if (n == 0 || n > seq_len)          /* undefined beyond the last term */
        return (seq_t)0;

    return seq_array[n - 1];
}
