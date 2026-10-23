/*
 * A191064  Primes p that have Kronecker symbol (p|22) = -1.
 * https://oeis.org/A191064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that Kronecker symbol (p|22) = -1 The
 *            function generates primes in order and selects those satisfying
 *            (p|2)*(p|11) = -1
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;

static seq_t *res_list = NULL;
static size_t res_cap   = 0;
static size_t res_cnt   = 0;

/* Ensure that the prime list has room for at least 'need' entries. */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Ensure that the result list has room for at least 'need' entries. */
static void ensure_res_cap(size_t need)
{
    if (need <= res_cap) return;
    size_t new_cap = res_cap ? res_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(res_list, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    res_list = tmp;
    res_cap  = new_cap;
}

/* Check the Kronecker symbol condition for an odd prime 'p'. 
   Since p is odd and not 11, (p|22) = (p|2)*(p|11). */
static bool cond(uint64_t p)
{
    /* (p|2) */
    uint8_t m8 = p & 7;
    int s2 = (m8 == 1 || m8 == 7) ? 1 : -1;  /* p ≡ 1,7 => +1; 3,5 => -1 */

    /* (p|11)  Legendre symbol */
    uint8_t m11 = p % 11u;
    int s11 = (m11 == 1 || m11 == 3 || m11 == 4 || m11 == 5 || m11 == 9) ? 1 : -1;

    return (s2 * s11) == -1;
}

/* Generate primes and fill res_list until it contains at least 'target' entries. */
static void generate_up_to(idx_t target)
{
    static uint64_t cur = 1;   /* last number examined, starts from 1 */
    while (res_cnt < target) {
        ++cur;
        if (cur == 2) {                 /* 2 is prime but not a result */
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = 2;
            continue;
        }
        if ((cur & 1) == 0) continue;   /* skip even numbers */

        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cur) break;
            if (cur % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)cur;

            if (cur != 11) {            /* 11 gives symbol 0 */
                if (cond(cur)) {
                    ensure_res_cap(res_cnt + 1);
                    res_list[res_cnt++] = (seq_t)cur;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defined only for n>=1 */
    generate_up_to((idx_t) n);
    return res_list[n - 1];
}
