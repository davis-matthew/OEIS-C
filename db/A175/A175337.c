/*
 * A175337  Fixed point of morphism 0 -> 00110, 1 -> 00111.
 * https://oeis.org/A175337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 * a(0) = 0;
 * For n > 0: let r = n % 5, p = n / 5.
 *   if r == 0 or r == 1 → a(n) = 0
 *   if r == 2 or r == 3 → a(n) = 1
 *   if r == 4           → a(n) = a(p)
 * This is derived from the morphism 0 → 00110, 1 → 00111.
 */

static seq_t a_rec(idx_t n) {
    idx_t r = n % 5u;
    idx_t parent = n / 5u;
    switch (r) {
        case 0:
        case 1:
            return 0;
        case 2:
        case 3:
            return 1;
        case 4:
            return a_rec(parent);
    }
    /* unreachable */
    return 0;
}

seq_t a(idx_t n) {
    return a_rec(n);
}
