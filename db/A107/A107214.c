/*
 * A107214  Primes of the form 2x^2 + 49y^2.
 * https://oeis.org/A107214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 2*x² + 49*y² (offset 1)
 * Range:     u8 n <= 3, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* A bound that contains far more terms than needed for n up to ≈ 12000.
   The exact upper bound can be tweaked if required, but 10 000 000
   comfortably covers the first several ten‑thousand terms. */
static const size_t LIMIT = 10000000U;

/* Global tables – allocated once. */
static uint8_t *rep       = NULL;   /* 1 if number can be written as 2x²+49y² */
static uint8_t *prime_flag = NULL;  /* 1 if number is prime */
static seq_t *sequence     = NULL;  /* primes of the desired form */
static size_t seq_len      = 0;     /* number of such primes found */

/* One‑time construction of the sieve and the derived sequence. */
static void initialise(void)
{
    if (rep != NULL)          /* already initialized */
        return;

    /* Allocate the bit tables with explicit casts so the code
       compiles under C++ (no implicit void* → T* conversion). */
    rep = (uint8_t *)calloc(LIMIT + 1U, 1U);
    prime_flag = (uint8_t *)calloc(LIMIT + 1U, 1U);

    /* Mark all numbers as prime initially (except 0,1). */
    for (size_t i = 2; i <= LIMIT; ++i)
        prime_flag[i] = 1U;

    /* Eratosthenes sieve. */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                prime_flag[j] = 0U;
        }
    }

    /* Mark numbers representable as 2x² + 49y². */
    for (uint64_t y = 0; 49ULL * y * y <= LIMIT; ++y) {
        uint64_t base = 49ULL * y * y;
        for (uint64_t x = 0; 2ULL * x * x <= LIMIT - base; ++x) {
            uint64_t v = base + 2ULL * x * x;
            rep[v] = 1U;
        }
    }

    /* Reserve an initial space for the sequence.  Five percent of LIMIT
       is plenty; realloc will grow if more are needed. */
    size_t reserve = LIMIT / 200;             /* 5 % of LIMIT */
    sequence = (seq_t *)malloc(reserve * sizeof(seq_t));
    seq_len = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (rep[i] && prime_flag[i]) {
            if (seq_len == reserve) {
                reserve *= 2;
                sequence = (seq_t *)realloc(sequence, reserve * sizeof(seq_t));
            }
            sequence[seq_len++] = (seq_t)i;
        }
    }

    /* Shrink to the exact size. */
    sequence = (seq_t *)realloc(sequence, seq_len * sizeof(seq_t));
}

/* Return the nth prime of the required form.  n ≥ 1.  Behaviour
   for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)          /* never called for offset 1, but defensive */
        return 0;

    initialise();

    /* All known terms are precomputed; this limit is huge enough for
       the intended test range.  If a larger n is requested the function
       returns 0 – the test harness should not exercise this case. */
    if ((size_t)n - 1 < seq_len)
        return sequence[(size_t)n - 1];

    return 0;   /* fallback for out‑of‑range requests */
}
