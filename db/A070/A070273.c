/*
 * A070273  Number of letters in the English names of the planets (in order
 *          from smallest orbit to largest) in 2002.
 * https://oeis.org/A070273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = number of letters in the English name of the nth planet
 *            by increasing orbital distance. 0 for n>9
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Planet names in order of increasing orbital distance (2002 data) */
    static const char *names[9] = {
        "Mercury",
        "Venus",
        "Earth",
        "Mars",
        "Jupiter",
        "Saturn",
        "Uranus",
        "Neptune",
        "Pluto"
    };
    if (n == 0) return 0;          /* out of range: return 0 */
    idx_t idx = n - 1;              /* convert to 0‑based index */
    if (idx >= 9) return 0;         /* beyond known planets: return 0 */

    const char *s = names[idx];
    seq_t len = 0;
    /* Count letters in the name */
    while (*s) { len++; s++; }

    return len;
}
