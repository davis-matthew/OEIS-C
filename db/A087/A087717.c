/*
 * A087717  Start with x=n, then iterate the map x -> A322982(x) with
 *          A322982(x)=2*x-1 if x is noncomposite, otherwise A322982(x) =
 *          A032742(x), the largest proper divisor of x. If this iteration
 *          leads to a fixed point then a(n) is the value of that fixed point.
 *          If the iteration leads to a cycle, a(n) is the...
 * https://oeis.org/A087717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value obtained by iterating the map * x -> (2x-1) if x=1
 *            or prime, * largest proper divisor of x otherwise, * until a
 *            cycle or fixed point is reached. * Return the smallest element
 *            of the cycle, * or the fixed point itself. If the process *
 *            never repeats, return 0. This definition * follow...
 * Range:     u8 n <= 78, u16 n <= 9808, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test if a 64‑bit integer is prime. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)    return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

/* Helper: smallest prime factor of a 64‑bit integer.
 * Assumes n >= 2.  For prime n it returns n itself. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)      return i;
        if (n % (i + 2) == 0) return i + 2;
    }
    return n;
}

/* One step of the iteration map A322982. */
static seq_t step(seq_t x)
{
    /* x is never 0 during calls. */
    if (x <= 1)  /* 1 is non‑composite */
        return (seq_t)1;          /* 2*1-1 =1 */
    /* Smallest prime factor is small for our domain, use 64‑bit helper. */
    uint64_t ux = (uint64_t)x;
    if (is_prime(ux)) {
        /* 2*x-1, careful with overflow which does not happen for
           the values we encounter (n ≤ 1e4 implies 2n-1 < 2e4). */
        return x + x - (seq_t)1;
    } else {
        uint64_t spf = smallest_prime_factor(ux);
        return x / (seq_t)spf;   /* largest proper divisor */
    }
}

/* Main OEIS function. */
seq_t a(idx_t n)
{
    seq_t start = n;
    /* Floyd cycle detection. */
    seq_t tortoise = step(start);
    seq_t hare     = step(step(start));
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare     = step(step(hare));
    }
    /* Find start of cycle. */
    tortoise = start;
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare     = step(hare);
    }
    seq_t cycle_start = tortoise;
    /* Determine the smallest element in the cycle. */
    seq_t min_cycle = cycle_start;
    seq_t temp = step(cycle_start);
    while (temp != cycle_start) {
        if (temp < min_cycle) min_cycle = temp;
        temp = step(temp);
    }
    return min_cycle;
}
