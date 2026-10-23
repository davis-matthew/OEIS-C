/*
 * A107412  Numbers n such that (n + prime(n)) and (n+1 + prime(n+1)) are
 *          divisible by 9.
 * https://oeis.org/A107412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that (n + prime(n)) and (n+1 + prime(n+1))
   are divisible by 9. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime precomputation ------------------------------------------------- */
static const size_t PRIMES_LIMIT = 2000000;          /* enough for n ≤ 10000   */
static seq_t *primes = NULL;                        /* array of primes       */
static size_t primes_count = 0;                     /* number of primes      */

/* Initialise the prime table once via a classic sieve. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    primes = (seq_t *)malloc(PRIMES_LIMIT * sizeof(seq_t));
    if (!primes) exit(EXIT_FAILURE);

    bool *composite = (bool *)malloc((PRIMES_LIMIT + 1) * sizeof(bool));
    if (!composite) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= PRIMES_LIMIT; ++i)
        composite[i] = false;

    size_t count = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!composite[i]) {
            primes[count++] = (seq_t)i;             /* prime(i)            */
            if (i * i <= PRIMES_LIMIT) {
                for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_count = count;
    free(composite);
    done = true;
}

/* --------------------------------------------------------------------- */
/* Sequence storage ----------------------------------------------------- */
static seq_t *seq = NULL;          /* array of sequence terms   */
static size_t seq_len   = 0;       /* number of terms stored    */
static size_t seq_cap   = 0;       /* capacity of the array     */
static size_t test_n    = 0;       /* last tested n             */

/* Grow seq array when needed. */
static void seq_add(seq_t value)
{
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap == 0 ? 4 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }
    seq[seq_len++] = value;
}

/* Generate sequence terms until at least 'target' terms are known. */
static void grow_seq(idx_t target)
{
    init_primes();   /* ensure prime table is ready */

    while (seq_len < (size_t)target) {
        ++test_n;                       /* candidate n (starting from 1) */

        /* Ensure primes table covers index test_n + 1. */
        if (primes_count <= (size_t)test_n + 1) {
            /* With PRIMES_LIMIT = 2 000 000 the needed prime indices are
               far below this value (prime(10001) < 200 000). */
            exit(EXIT_FAILURE);
        }

        /* Compute (n + prime(n)) mod 9 and (n+1 + prime(n+1)) mod 9. */
        seq_t p_n   = primes[test_n - 1];   /* prime(n)  */
        seq_t p_np1 = primes[test_n];       /* prime(n+1)*/
        seq_t v1 = (seq_t)test_n + p_n;
        seq_t v2 = (seq_t)(test_n + 1) + p_np1;

        if (v1 % 9 == 0 && v2 % 9 == 0)
            seq_add((seq_t)test_n);
    }
}

/* --------------------------------------------------------------------- */
/* Public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    if (seq == NULL) {
        seq = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }
    grow_seq(n);
    return seq[(size_t)n - 1];
}
