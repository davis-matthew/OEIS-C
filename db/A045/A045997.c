/*
 * A045997  Number of iterations required to reach stationary value when
 *          applying repeatedly applying d, the number of divisors function,
 *          to n!.
 * https://oeis.org/A045997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations required to reach the fixed point *
 *            (1 or 2) by repeatedly applying the divisor-count function d to
 *            n!. * Example: a(3) = 3 because 3! = 6 → d(6)=4 → d(4)=3 →
 *            d(3)=2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* ---------- small sieve to generate primes up to a fixed limit --------- */
#define LIMIT_PRIMES 20000          /* sufficient for all n in tests          */
#define MAX_PRIMES   2500           /* max number of primes ≤ LIMIT_PRIMES   */
static bool  initialized = false;
static uint64_t primes[MAX_PRIMES];
static int     prime_idx_of[LIMIT_PRIMES + 1];
static int     prime_cnt = 0;

static void init_sieve(void)
{
    if (initialized)
        return;
    bool composite[LIMIT_PRIMES + 1] = { false };
    composite[0] = composite[1] = true;
    for (idx_t i = 2; i * i <= LIMIT_PRIMES; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= LIMIT_PRIMES; j += i)
                composite[j] = true;
        }
    }
    for (idx_t i = 2; i <= LIMIT_PRIMES; ++i) {
        if (!composite[i]) {
            prime_idx_of[(int)i] = prime_cnt;
            primes[prime_cnt++] = i;
        } else {
            prime_idx_of[(int)i] = -1;
        }
    }
    initialized = true;
}

/* ---------- helper: factor an integer 'val' (≤ LIMIT_PRIMES) and add the
 *          exponents to the array 'exp_arr' (indexed by prime index).
 *  exp_arr must have size >= prime_cnt. --------------------------------- */
static void factor_and_accumulate(idx_t val, seq_t *exp_arr)
{
    idx_t tmp = val;
    for (int pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        idx_t q = primes[pi];
        if (tmp % q == 0) {
            int cnt = 0;
            while (tmp % q == 0) {
                tmp /= q;
                ++cnt;
            }
            exp_arr[pi] += cnt;
        }
    }
    if (tmp > 1) {           /* tmp is prime */
        int idx = prime_idx_of[(int)tmp];
        exp_arr[idx] += 1;
    }
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;                /* n! = 1 or 2, already fixed point           */

    init_sieve();

    /* arrays to hold prime exponents of the current number             */
    static seq_t curr_exp[MAX_PRIMES];
    static seq_t next_exp[MAX_PRIMES];
    (void)prime_cnt;    /* silence unused variable warning for some builds */

    /* --------- compute exponents of n! --------------------------------- */
    for (int i = 0; i < prime_cnt; ++i)
        curr_exp[i] = 0;

    for (int i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n)
            break;
        /* Legendre formula for e_i */
        seq_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p)
                break;           /* prevent overflow */
            power *= p;
        }
        /* factor e+1 and add to current exponent array */
        factor_and_accumulate((idx_t)(e + (seq_t)1), curr_exp);
    }

    seq_t steps = 1;          /* first application of d has produced curr_exp */

    /* --------- iterate until the number is 1 or 2 --------------------- */
    while (true) {
        /* check if the number is 1 (all exponents zero) */
        bool all_zero = true;
        for (int i = 0; i < prime_cnt; ++i) {
            if (curr_exp[i]) {
                all_zero = false;
                break;
            }
        }
        if (all_zero)
            return steps;      /* reached 1 */

        /* check if the number is 2 (only exponent of prime 2 is 1) */
        bool is_two = false;
        if (curr_exp[0] == 1) {
            bool other_nonzero = false;
            for (int i = 1; i < prime_cnt; ++i) {
                if (curr_exp[i]) {
                    other_nonzero = true;
                    break;
                }
            }
            is_two = !other_nonzero;
        }
        if (is_two)
            return steps;      /* reached 2 */

        /* compute next exponent array from current exponents */
        for (int i = 0; i < prime_cnt; ++i)
            next_exp[i] = 0;

        for (int i = 0; i < prime_cnt; ++i) {
            if (curr_exp[i] == 0)
                continue;
            idx_t val = (idx_t)(curr_exp[i] + (seq_t)1);   /* e_i + 1   */
            factor_and_accumulate(val, next_exp);
        }

        /* prepare for next iteration */
        for (int i = 0; i < prime_cnt; ++i)
            curr_exp[i] = next_exp[i];
        ++steps;
    }
}
