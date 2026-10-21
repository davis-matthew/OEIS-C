/*
 * A068499  Numbers m such that m! reduced modulo (m+1) is not zero.
 * https://oeis.org/A068499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068499
   Numbers m such that m! mod (m+1) ≠ 0.
   Closed form: a(1)=1, a(2)=2, a(3)=3,
   a(n)=prime(n‑1)−1 for n≥4.
   The implementation keeps a static list of primes and grows it
   lazily when the required prime index is not yet available. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static storage for the prime list */
static unsigned int *prime_list = NULL;   /* array of primes */
static size_t     prime_cap   = 0;         /* current capacity of prime_list */
static size_t     prime_cnt   = 0;         /* number of primes stored */

/* ensure we have at least `need` primes in prime_list (prime(need) exists) */
static void generate_primes_upto(size_t need)
{
    if (prime_cnt >= need) return;           /* already enough */

    /* enlarge storage if needed */
    if (prime_cap < need) {
        unsigned int *tmp = (unsigned int *)realloc(prime_list,
                                                    need * sizeof(unsigned int));
        if (!tmp) { /* memory failure – leave function void */ return; }
        prime_list = tmp;
        prime_cap  = need;
    }

    /* if this is the first prime we must add 2 first */
    if (prime_cnt == 0) {
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* start from the next candidate above the last prime */
    unsigned int candidate = prime_list[prime_cnt - 1] + 1;
    if (candidate <= 2) candidate = 3;
    if ((candidate & 1) == 0) candidate++;   /* make odd */

    /* generate primes until we reach the needed amount */
    while (prime_cnt < need) {
        unsigned int sqrt_c = (unsigned int)sqrt((double)candidate);
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_c; ++i) {
            if (candidate % prime_list[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
            if (prime_cnt >= need) break;
        }
        candidate += 2;           /* skip even numbers */
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safety */

    /* small indices are fixed manually */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* for n >= 4 we need the (n-1)-th prime */
    idx_t prime_index = n - 1;            /* 1‑based prime index */
    generate_primes_upto((size_t)prime_index);   /* ensure existence */

    /* prime_list is 0‑based: prime(k) == prime_list[k-1] */
    unsigned int p = prime_list[(size_t)prime_index - 1];
    return (seq_t)p - (seq_t)1;
}
