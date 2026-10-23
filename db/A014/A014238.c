/*
 * A014238  a(n) = (n-th number that is 1 or prime) - (n-th composite number).
 * https://oeis.org/A014238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-th number that is 1 or prime) - (n-th composite
 *            number)
 * Range:     i8 n <= 43, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#if defined(__cplusplus)
#include <new>
#endif

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* Internal helper to assign an integer to a seq_t element in a
   way that works for both primitive and wide/big integer types. */
static void seq_assign_int(seq_t *dst, idx_t val)
{
#if defined(__cplusplus)
    new (dst) seq_t(val);
#else
    *dst = (seq_t)val;
#endif
}

/* ------------------------------------------------------------------ */
/*  Sieve and sequence data                                         */
/* ------------------------------------------------------------------ */

static bool built = false;          /* have the tables been built? */
static const idx_t SIEVE_LIMIT = 200000; /* static limit sufficient for n up to ~10000 */

/* Arrays for primes and composites, allocated lazily */
static seq_t *primes = NULL;
static size_t prime_count = 0;
static seq_t *composites = NULL;
static size_t comp_count = 0;

/* Build the sieve and fill the primes[] and composites[] arrays */
static void init_sequence(void)
{
    if (built) return;

    idx_t N = SIEVE_LIMIT;
    /* Allocate a temporary boolean array for the sieve */
    bool *is_prime = (bool *)malloc((size_t)(N + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    /* Initialize sieve */
    for (idx_t i = 0; i <= N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= N; q += p) is_prime[q] = false;
        }
    }

    /* Count primes and composites */
    for (idx_t i = 2; i <= N; ++i) if (is_prime[i]) ++prime_count;
    for (idx_t i = 4; i <= N; ++i) if (!is_prime[i]) ++comp_count;

    /* Allocate arrays */
    primes = (seq_t *)malloc(prime_count * sizeof(seq_t));
    composites = (seq_t *)malloc(comp_count * sizeof(seq_t));
    if (!primes || !composites) exit(EXIT_FAILURE);

    /* Fill arrays with proper construction */
    size_t p_idx = 0, c_idx = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (is_prime[i]) seq_assign_int(&primes[p_idx++], i);
    }
    for (idx_t i = 4; i <= N; ++i) {
        if (!is_prime[i]) seq_assign_int(&composites[c_idx++], i);
    }

    free(is_prime);
    built = true;
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  Calls with n == 0
       return 0 as a harmless placeholder. */
    if (n == 0) return (seq_t)0;

    init_sequence();

    seq_t prime_val;
    if (n == 1) {
        prime_val = (seq_t)1;          /* the only “1 or prime” preceding the primes */
    } else {
        /* primes[0] = 2 is the 2nd “1 or prime” number */
        prime_val = primes[(size_t)n - 2];
    }
    seq_t comp_val = composites[(size_t)n - 1];  /* composites[0] = 4 is the 1st composite */

    return prime_val - comp_val;
}
