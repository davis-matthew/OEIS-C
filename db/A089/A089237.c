/*
 * A089237  List of primes and squares.
 * https://oeis.org/A089237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th smallest non‑negative integer that is either a prime
 *            or a perfect square (0 is considered 0²); the terms are sorted
 *            in increasing order and start with a(1)=0
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static storage for the precomputed list of terms */
static seq_t *sequence = NULL;
static size_t sequence_len = 0;

/* initialise the table up to a fixed bound (enough for n ≤ 10000) */
static void init_sequence(void)
{
    /* bound chosen so that the count of primes + squares ≳ 40000 */
    const idx_t MAX_B = 500000;          /* also safe for larger n */
    bool *isPrime = (bool*)calloc(MAX_B + 1, sizeof(bool));
    if (!isPrime) exit(EXIT_FAILURE);

    /* sieve of Eratosthenes */
    for (idx_t i = 2; i <= MAX_B; ++i) isPrime[i] = true;
    for (idx_t i = 2; i * i <= MAX_B; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= MAX_B; j += i)
                isPrime[j] = false;
        }
    }

    /* mark perfect squares */
    bool *isSquare = (bool*)calloc(MAX_B + 1, sizeof(bool));
    if (!isSquare) { free(isPrime); exit(EXIT_FAILURE); }
    for (idx_t j = 0; j * j <= MAX_B; ++j)
        isSquare[j * j] = true;

    /* build the sorted list of terms */
    size_t capacity = 128;
    seq_t *terms = (seq_t*)malloc(capacity * sizeof(seq_t));
    if (!terms) { free(isPrime); free(isSquare); exit(EXIT_FAILURE); }

    size_t cnt = 0;
    for (idx_t i = 0; i <= MAX_B; ++i) {
        if (i == 0 || isPrime[i] || isSquare[i]) {
            if (cnt == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t*)realloc(terms, capacity * sizeof(seq_t));
                if (!tmp) { free(isPrime); free(isSquare); free(terms); exit(EXIT_FAILURE); }
                terms = tmp;
            }
            terms[cnt++] = (seq_t)i;
        }
    }

    /* store the result */
    sequence      = terms;
    sequence_len  = cnt;

    /* free temporary arrays */
    free(isPrime);
    free(isSquare);
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (!sequence) init_sequence();

    if (n == 0) return 0;              /* safety guard – not needed for offset 1 */

    /* if request exceeds pre‑computed range we could extend the sieve,
       but n up to 10 000 is guaranteed to be within our bound */
    if ((idx_t)n > sequence_len) {
        /* out of range – behavior unspecified; return 0 as a fallback */
        return 0;
    }
    return sequence[(size_t)(n - 1)];
}
