/*
 * A076695  Dimension of S2(G0(p)) where p runs through the odd primes >= 13,
 *          G0(N) is the Hecke subgroup of SL2(Z), consisting of 2 X 2
 *          matrices (a,b; c,d) with c == 0 (mod N).
 * https://oeis.org/A076695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let p(n) be the nth odd prime ≥13 (so p(1)=13, p(2)=17, …). Then
 *            a(n) = (p(n)+1)/12 if p(n) ≡ 11 (mod 12) // -1 (mod 12) a(n) =
 *            (p(n)-5)/12 if p(n) ≡ 5 (mod 12) a(n) = (p(n)-7)/12 if p(n) ≡ 7
 *            (mod 12) a(n) = (p(n)-13)/12 if p(n) ≡ 1 (mod 12) All divisions
 *            are integer divisions
 * Range:     u8 n <= 49, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static int sieve_limit = 200000;   /* covers primes up to the 17984th prime */
static int *sieve = NULL;
static int *prime_list = NULL;
static size_t prime_cnt = 0;
static int initialized = 0;

/* Simple sieve of Eratosthenes, stores all primes ≤ sieve_limit. */
static void init_primes(void)
{
    if (initialized)
        return;

    /* Allocate arrays */
    sieve = (int *)malloc((sieve_limit + 1) * sizeof(int));
    prime_list = (int *)malloc(20000 * sizeof(int)); /* enough for 20000 primes */

    for (int i = 0; i <= sieve_limit; ++i)
        sieve[i] = 1;

    sieve[0] = sieve[1] = 0;

    for (int i = 2; i * i <= sieve_limit; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= sieve_limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Collect primes */
    prime_cnt = 0;
    for (int i = 2; i <= sieve_limit; ++i) {
        if (sieve[i]) {
            if (prime_cnt >= 20000) /* overflow guard; should not happen */
                break;
            prime_list[prime_cnt++] = i;
        }
    }

    initialized = 1;
}

/* Return the nth odd prime ≥13 (n is 1‑based). */
static seq_t nth_prime_from_13(idx_t n)
{
    /* For n=1 we need the 6th prime (index 5). */
    idx_t index = n + 4;  /* convert to 0‑based index of prime_list */
    if (index >= prime_cnt) {
        /* Safety: this shouldn't happen for the expected limits. */
        return (seq_t)-1;
    }
    return (seq_t)prime_list[index];
}

seq_t a(idx_t n)
{
    init_primes();

    /* n < 1 is undefined; return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    seq_t p = nth_prime_from_13(n);
    if (p == (seq_t)-1)
        return (seq_t)0;   /* overflow case */

    int rem = (int)(p % 12);

    if (rem == 11)      /* p ≡ -1 (mod 12) */
        return (p + 1) / 12;
    else if (rem == 5)  /* p ≡ 5 (mod 12) */
        return (p - 5) / 12;
    else if (rem == 7)  /* p ≡ 7 (mod 12) */
        return (p - 7) / 12;
    else if (rem == 1)  /* p ≡ 1 (mod 12) */
        return (p - 13) / 12;
    else
        return (seq_t)0;   /* should never happen */
}
