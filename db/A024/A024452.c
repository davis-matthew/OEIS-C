/*
 * A024452  a(n) = [ (2nd elementary symmetric function of P(n))/(first
 *          elementary symmetric function of P(n)) ], where P(n) = {first n+1
 *          primes}.
 * https://oeis.org/A024452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ (∑_{i<j} p_i p_j) / (∑ p_i) ⌋ where the set {p_i} is
 *            the first n+1 primes
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Static storage for primes and their prefix sums of first and second
    elementary symmetric functions.  */
static seq_t *Primes   = NULL;   /* Primes[0..cnt-1]                     */
static seq_t *Sum1     = NULL;   /* prefix sums of primes               */
static seq_t *Sum2     = NULL;   /* prefix sums of pairwise products    */
static size_t PrimesCnt = 0;     /* number of primes currently stored   */
static size_t Capacity  = 0;     /* current array capacity              */
static uint64_t NextCand  = 3;   /* next odd integer to test for primality */
static int    Initialized = 0;   /* initialization flag                 */

/*  Ensure that the storage arrays can hold at least 'needed' elements.  */
static void grow_storage(size_t needed)
{
    if (needed <= Capacity) return;
    size_t new_cap = Capacity ? Capacity : 1;
    while (new_cap < needed) new_cap <<= 1;

    Primes = (seq_t *)realloc(Primes, new_cap * sizeof(seq_t));
    Sum1   = (seq_t *)realloc(Sum1,   new_cap * sizeof(seq_t));
    Sum2   = (seq_t *)realloc(Sum2,   new_cap * sizeof(seq_t));

    Capacity = new_cap;
}

/*  Lazy initializer for the first prime.  */
static void init_once(void)
{
    if (Initialized) return;
    grow_storage(1);
    Primes[0] = (seq_t)2;
    Sum1[0]   = (seq_t)2;
    Sum2[0]   = (seq_t)0;
    PrimesCnt = 1;
    NextCand  = 3;
    Initialized = 1;
}

/*  Add a new prime to the storage, updating the two prefix sums.  */
static void add_prime(uint64_t cand)
{
    seq_t p = (seq_t)cand;
    seq_t prevS1 = PrimesCnt ? Sum1[PrimesCnt-1] : (seq_t)0;
    seq_t prevS2 = PrimesCnt ? Sum2[PrimesCnt-1] : (seq_t)0;

    Sum1[PrimesCnt] = prevS1 + p;
    Sum2[PrimesCnt] = prevS2 + p * prevS1;
    Primes[PrimesCnt] = p;
    PrimesCnt++;
}

/*  Generate primes until we have at least 'needed' of them.  */
static void generate_primes_up_to(size_t needed)
{
    init_once();
    if (PrimesCnt >= needed) return;
    grow_storage(needed);

    while (PrimesCnt < needed) {
        uint64_t cand = NextCand;

        int is_prime = 1;
        for (size_t i = 0; i < PrimesCnt; ++i) {
            uint64_t pr = (uint64_t)Primes[i];
            if (pr > cand / pr) break;   /* pr*pr would overflow or > cand */
            if (cand % pr == 0) { is_prime = 0; break; }
        }

        if (is_prime) add_prime(cand);
        NextCand += 2;   /* skip even numbers */
    }
}

/*  Function required by the test harness.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* behaviour below offset is irrelevant */

    size_t need = (size_t)n + 1;       /* we need the first (n+1) primes */
    generate_primes_up_to(need);

    seq_t e1 = Sum1[need-1];           /* first elementary symmetric sum */
    seq_t e2 = Sum2[need-1];           /* second elementary symmetric sum */
    return e2 / e1;                    /* integer division (floor)      */
}
