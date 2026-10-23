/*
 * A102770  (p*q - 1)/2 where p and q are consecutive odd primes.
 * https://oeis.org/A102770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+1) * prime(n+2) - 1) / 2 where prime(k) is the
 *            k‑th prime (prime(1) = 2)
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the code is valid C11 and C++17.  It uses only the types
   seq_t and idx_t that are provided by the build, and it obtains
   primes on demand.  The implementation keeps an internal array of
   primes stored as uint64_t because for n≈10 000 the 10 002‑nd prime
   is 104 729 and fits easily in 64 bits.  The arithmetic of the
   return value is performed with seq_t operands so it works for
   arbitrary‑precision builds as well. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Prime generation                                                       */
/* ------------------------------------------------------------------ */

static uint64_t *prime_buf = NULL;   /* array of primes (0‑based)           */
static size_t    prime_cnt = 0;      /* number of primes stored              */

/* Ensure that prime_buf contains at least `need` primes. */
static void
generate_primes_up_to(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Rosser's bound: for n ≥ 6, p_n < n(ln n + ln ln n) */
    double dn = (double)need;
    uint64_t limit;
    if (need < 6)
        limit = 13;                     /* 6th prime is 13 */
    else
        limit = (uint64_t)(dn * (log(dn) + log(log(dn))) + 3.0);

    /* Sieve of Eratosthenes up to `limit` */
    unsigned char *is_comp = (unsigned char *)calloc((size_t)limit + 1U, 1U);
    if (!is_comp) {
        /* Very unlikely, but guard against OOM. */
        exit(EXIT_FAILURE);
    }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            ++cnt;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    /* Allocate a new buffer of the exact size needed. */
    uint64_t *new_buf = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_buf) {
        free(is_comp);
        exit(EXIT_FAILURE);
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i])
            new_buf[idx++] = i;

    free(is_comp);

    /* Replace old buffer, if any. */
    if (prime_buf)
        free(prime_buf);
    prime_buf = new_buf;
    prime_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                      */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                    /* value outside offset is undefined */

    /* We need primes at positions n+1 and n+2, i.e. indices n and n+1
       in the 0‑based prime array.  So require at least (n+2) primes. */
    generate_primes_up_to((size_t)n + 2U);

    seq_t p1 = (seq_t)prime_buf[(size_t)n];     /* prime(n+1)   */
    seq_t p2 = (seq_t)prime_buf[(size_t)n + 1U];/* prime(n+2)   */

    seq_t prod = p1 * p2;                      /* multiplication of seq_t operands */
    seq_t res  = (prod - (seq_t)1) / (seq_t)2;

    return res;
}
